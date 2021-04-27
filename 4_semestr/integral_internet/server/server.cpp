#include "integral_internet.h"

struct clients_information
{
  int socket;
  unsigned num_threads;
};

int FindComputers()
{
  int broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);
  Assert(broadcast_socket >= 0);
  
  int enable = 1;
  Assert(setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable)) == 0);
  /*struct sockaddr_in clientaddr{};
  clientaddr.sin_family = AF_INET;
  clientaddr.sin_port = htons(ClientFindPort);
  clientaddr.sin_addr = htonl(INADDR_BROADCAST);
  
  Assert(bind(broadcast_socket, (struct sockaddr_in*)&clientaddr, sizeof(clientaddr)) >= 0);
  */
  struct sockaddr_in servaddr{};
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(ServerFindPort_u);
  servaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

  Assert(sendto(broadcast_socket, FindMsg, FindMsgSize, MSG_CONFIRM, (const struct sockaddr*) &servaddr, sizeof(servaddr)) != -1);
  
  printf("Message sent\n");

  close(broadcast_socket);
  return 0;
}

int StartListenClients(unsigned n_comp)
{
  int listen_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
  Assert(listen_socket > 0);

  int enable = 1;
  Assert(setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == 0);

  struct sockaddr_in listen_addr{};
  listen_addr.sin_family = AF_INET;
  listen_addr.sin_port = htons(ListenPort_u);
  listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  Assert(bind(listen_socket, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) >= 0);

  Assert(listen(listen_socket, (int)n_comp) >= 0);

  return listen_socket;
}

unsigned AcceptComputers(int listen_socket, unsigned n_comp, int* sockets)
{
  Assert(sockets);

  unsigned real_comps = 0;
  for (unsigned i = 0; i < n_comp; ++i)
  {
    fd_set select_set{};
    FD_ZERO(&select_set);
    FD_SET(listen_socket, &select_set);

    int ready = 0;
    Assert((ready = select(listen_socket + 1, &select_set, NULL, NULL, &Wait_time)) >= 0);
    if (ready == 0)
      break;

    int new_socket = accept(listen_socket, NULL, NULL);
    if (new_socket < 0 && errno == EAGAIN)
    {
      printf("WTF in AcceptComputers\n");
      exit(-1);
      continue;
    }
    Assert(new_socket > 0);
    ImproveTCPSocket(new_socket);

    sockets[real_comps] = new_socket;
    real_comps++;
  }
  
  return real_comps;
}

unsigned GetClientsInf(struct clients_information* clients_inf, unsigned n_comp_find, int* clients_sockets)
{
  Assert(clients_sockets);
  Assert(clients_inf);
  unsigned n_find = 0;
  for (unsigned i = 0; i < n_comp_find; ++i)
  {
    char tmp_buf[MaxBufLen];
    
    PrintSocket(clients_sockets[i]);

    int recv_bytes = (int)recv(clients_sockets[i], &tmp_buf, MaxBufLen, 0);
    if (recv_bytes == -1 && errno == ECONNREFUSED)
    {
      printf("Socket has been killed(((\n");
      continue;
    }
    Assert(recv_bytes > 0);

    clients_inf[n_find].socket = clients_sockets[i];
    clients_inf[n_find].num_threads = (unsigned)atoi(tmp_buf);
    
    printf("Socket: %2d -> num threads = %u\n", clients_inf[n_find].socket, clients_inf[n_find].num_threads);    
    
    n_find++;
  }
  return n_find;
}

void SendTasks(struct clients_information* clients_inf, unsigned connected_comps)
{
  Assert(clients_inf);

  unsigned total_threads = 0;
  for (unsigned i = 0; i < connected_comps; ++i)
    total_threads += clients_inf[i].num_threads;
  
  unsigned long long gived_steps = 0;
  double previous_stop = 0;
  for (unsigned i = 0; i < connected_comps; ++i)
  {
    double cli_coef = double(clients_inf[i].num_threads) / double(total_threads);
    printf("cli_coef: %lg\n", cli_coef);

    struct integral_message int_msg{};
    
    if (i == 0)
    {
      int_msg.n_steps = (unsigned long long)(double(TotalStepsI) * cli_coef);
      int_msg.start   = StartI;
      int_msg.stop    = StartI + (StopI - StartI) * cli_coef;
    }
    else if (i == connected_comps - 1)
    {
      int_msg.n_steps = TotalStepsI - gived_steps;
      int_msg.start   = previous_stop;
      int_msg.stop    = StopI; 
    }
    else
    {
      int_msg.n_steps = (unsigned long long)(double(TotalStepsI) * cli_coef);
      int_msg.start   = previous_stop;
      int_msg.stop    = StartI + (StopI - StartI) * cli_coef;
    }

    if (SendSocketText(clients_inf[i].socket, (char*)&int_msg, sizeof(int_msg)) == -1)
    {
      printf("Clinet with socket: %d died\n", clients_inf[i].socket);
      exit(-1); 
    }
    previous_stop = int_msg.stop;
    gived_steps += int_msg.n_steps;

    printf("Number %u from %u:\nStart: %lg\nStop: %lg\nN_steps: %llu\n------------------\n", \
    i, connected_comps, int_msg.start, int_msg.stop, int_msg.n_steps);
  }
}


int main(int argc, char** argv)
{
  if (argc != 2)
  {
    printf("Incorrect input\nRun with desireable number of computers\n");
    return -1;
  }
  unsigned n_comp = unsigned(atoi(argv[1]));

  int listen_socket = StartListenClients(n_comp);
  FindComputers();
  
  int* clients_sockets = (int*) calloc(n_comp, sizeof(clients_sockets[0]));
  unsigned n_comp_find = AcceptComputers(listen_socket, n_comp, clients_sockets);
  printf("Finded: %u\n", n_comp_find);
  
  struct clients_information* clients_inf = (struct clients_information*)calloc(n_comp_find, sizeof(clients_inf[0]));
  Assert(clients_inf);

  unsigned connected_comps = GetClientsInf(clients_inf, n_comp_find, clients_sockets);
  SendTasks(clients_inf, connected_comps);

  free(clients_inf);
  free(clients_sockets);
  return 0;
}
