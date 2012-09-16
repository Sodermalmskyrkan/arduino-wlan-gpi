#include <SPI.h>
//#include <Dhcp.h>
//#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
//#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <util.h>

// MAC address just has to be unique. This should work
byte mac[] = { 
  0x90, 0xA2, 0xDA, 0x0D, 0x51, 0xDC };
// The IP address will be dependent on your local network:
IPAddress ip(192, 168, 2, 250);
unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char ReciveBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

// Define input output pins
int ledRed = 7;
int ledYellow = 8;
int ledGreen = 9;


void setup () {
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);

  Serial.begin(9600);

  // Setup inputs/outputs
  pinMode(ledRed, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen, OUTPUT);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();

  if(packetSize)
  {

    UdpRecive(packetSize);

    char UdpSendBuffer[] = "acknowledged";       // a string to send back
    UdpSend(UdpSendBuffer);

    delay(10);
  }
}

void UdpRecive(int packetSize)
{
  // read the packet into packetBufffer
  Udp.read(ReciveBuffer,UDP_TX_PACKET_MAX_SIZE);
  Serial.println("Contents:");
  Serial.println(ReciveBuffer);

  Serial.print("Received packet of size ");
  Serial.println(packetSize);
  Serial.print("From ");
  IPAddress remote = Udp.remoteIP();
  for (int i =0; i < 4; i++)
  {
    Serial.print(remote[i], DEC);
    if (i < 3)
    {
      Serial.print(".");
    }
  }
  Serial.print(", port ");
  Serial.println(Udp.remotePort());
  String rb (char(ReciveBuffer[0]));
  if (rb ='r')
  {
    digitalWrite(ledRed, HIGH);
  }
  /*  for(int i=0; i < packetSize; i++) 
   {
   Serial.print(char(ReciveBuffer[i]));
   }*/
  /*  if (ReciveBuffer[1]='r')
   { 
   digitalWrite(ledRed, HIGH);
   }*/
}

void UdpSend(char UdpSendBuffer[])
{
  // send a reply, to the IP address and port that sent us the packet we received
  Serial.println("Send:");
  Serial.println(UdpSendBuffer);
  Udp.beginPacket(Udp.remoteIP(), 8888);
  Udp.write(UdpSendBuffer);
  Udp.endPacket(); 
}











