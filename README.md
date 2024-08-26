Network Programming  </br>
Language: C  </br>  </br>

Task 1 Unix Domain Protocol – Connectionless </br>
1.	Modify both Unix Domain Protocol – Connectionless server and client program so that the server sends the received message back to the client. </br></br>

Task 2 Unicast and Multicast programs </br>
1.	You are required to modify three (3) programs as follows: </br>
  a.	unicast_client.c: (refer to Lab 1 udp_client.c) </br>
    i.	This program will send a unicast message to the server.  </br>
    ii.	It will repeatedly request user input until the user types “Good bye”. </br>
  b.	multicast_sender.c: (refer to Lab 4 multicast_sender.c) </br>
    i.	This program will receive a unicast message from the unicast client. </br>
    ii.	It will then forward the received message to the multicast receiver(s) who joined the multicast group. </br>
  c.	multicast_receiver.c: (refer to Lab 4 multicast_receiver.c) </br>
    i.	This program will receive a multicast message from the server if it has correctly joined the designated multicast group.  </br>
    ii.	This program will NOT receive a multicast message from the server if it has joined the incorrect multicast group.  </br>
2.	The unicast IP address, unicast port number, multicast IP address, and multicast port number are specified when the program is executed in the terminal.  </br>
    -E.g., ./m_s 224.0.0.15 192.168.56.101 5000  </br>
    -Multicast IP address: 224.0.0.15  </br>
    -Multicast Port Number: 5000  </br>
    -Unicast IP address: 192.168.56.101  </br>
    -Unicast Port Number: 4999 (multicast port number – 1)  </br>
3.	To verify if your programs are running correctly, below are the steps.  </br>
  a.	Start one (1) server in a new terminal by using the following commands:  </br>
    ./m_s 224.0.0.15 192.168.56.101 5000  </br>
  b.	Start one (1) sender in a new terminal by using the following commands:  </br>
    ./u_c  </br>
  c.	Open three (3) separate terminals and start three (3) receivers using following commands:  </br>
    ./m_r 224.0.0.15 192.168.56.101 5000   </br>
    ./m_r 224.0.0.15 192.168.56.101 5000   </br>
    ./m_r 224.0.0.25 192.168.56.101 4998   </br>
