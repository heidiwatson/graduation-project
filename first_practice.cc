/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* studied by heidiwatson */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");
// Define a Log component with a specific name "FirstScriptExample"

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS); 
  // set the time resolution to one nanosecond -> default value
  // resolution value means the smallest value that can be represented
  // we can change the resolution exactly once
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  // logging at INFO level

  NodeContainer nodes;
  nodes.Create (2); // create two nodes
  // generate the Node 

  // simplest point-to-point
  PointToPointHelper pointToPoint;
  // PointToPointHelper configure and connect PointToPointNetDevice and PointToPointChannel
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  // datarate : "5Mbps" -- PointToPointNetDevice
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  // propagation dealy : "2ms"

  NetDeviceContainer devices;
  // list of all of the NetDevice objects
  devices = pointToPoint.Install (nodes); // NetDeviceContainer create
  // finish configuring the device and channel

  InternetStackHelper stack;
  stack.Install (nodes);
  // install an Internet stack(TCP UDP IP ..) on each of the nodes container

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0"); // network, mask

  Ipv4InterfaceContainer interfaces = address.Assign (devices);
  // actual address assignment
  // interfaces: list of net devices whicha are associated with an IP addr

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  // install a UdpEchoServerApplication on the nodes
  serverApps.Start (Seconds (1.0)); // start time
  serverApps.Stop (Seconds (10.0)); // stop time

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  // remote address and remote port
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  // tell the client that the maximum number of packets we allow it to send
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  // tell the client how long to wait between packets
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  // tell the client how large its packet playloads should be

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  // client start after the server, 1.0(start) + 1.0(resuolution time)
  clientApps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
