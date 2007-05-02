// -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*-
//
// Copyright (c) 2006 Georgia Tech Research Corporation
// All rights reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// Author: George F. Riley<riley@ece.gatech.edu>
//

// Implement the basic Node object for ns3.
// George F. Riley, Georgia Tech, Fall 2006

#include "node.h"
#include "node-list.h"
#include "net-device.h"
#include "ns3/simulator.h"

namespace ns3{

Node::Node()
  : m_id(0), m_sid(0)
{
}

Node::Node(uint32_t sid)
  : m_id(0), m_sid(sid)
{ 
}
  
Node::~Node ()
{}


uint32_t 
Node::GetId (void) const
{
  return m_id;
}

void   
Node::SetId(uint32_t id )
{
  m_id = id;
}

uint32_t 
Node::GetSystemId (void) const
{
  return m_sid;
}

void   
Node::SetSystemId(uint32_t s )
{
  m_sid = s;
}

uint32_t 
Node::AddDevice (NetDevice *device)
{
  device->Ref ();
  uint32_t index = m_devices.size ();
  m_devices.push_back (device);
  DoAddDevice (device);
  return index;
}
NetDevice *
Node::GetDevice (uint32_t index) const
{
  return m_devices[index];
}
uint32_t 
Node::GetNDevices (void) const
{
  return m_devices.size ();
}

void Node::Dispose()
{
  for (std::vector<NetDevice *>::iterator i = m_devices.begin ();
       i != m_devices.end (); i++)
    {
      NetDevice *device = *i;
      device->Dispose ();
      device->Unref ();
    }
  m_devices.erase (m_devices.begin (), m_devices.end ());
}

Node::SmartNodeVec_t **
Node::GetNodeVector (void)
{
  static SmartNodeVec_t *vector = 0;
  if (vector == 0)
    {
      vector = new SmartNodeVec_t ();
      Simulator::ScheduleDestroy (&Node::DestroyNodes);
    }
  return &vector;
}

Node::SmartNodeVec_t **
Node::GetPrototypeVector (void)
{
  static SmartNodeVec_t *vector = 0;
  if (vector == 0)
    {
      vector = new SmartNodeVec_t ();
      Simulator::ScheduleDestroy (&Node::DestroyPrototypes);
    }
  return &vector;
}

void
Node::DestroyNodes (void)
{
  SmartNodeVec_t **vector = GetNodeVector ();
  delete *vector;
  *vector = 0;
}
void
Node::DestroyPrototypes (void)
{
  SmartNodeVec_t **vector = GetPrototypeVector ();
  delete *vector;
  *vector = 0;
}

// Node stack creation and management routines.
Node* Node::Create()
{
  Node* n = (*GetPrototypeVector ())->Back()->Copy(); // Copy the top of the stack
  (*GetNodeVector ())->Add (n);
  NodeList::Add (n);           // Add to global list of nodes
  return n;
}

Node* Node::Create(uint32_t sid)
{ // Create with distributed simulation systemid
  // ! Need to check if sid matches DistributedSimulator system id,
  // and create an empty (ghost) node if so.  Code this later
  Node* n = Create(sid);
  return n;
}

Node* Node::GetNodePrototype()
{ // Get node* to top of prototypes stack
  return (*GetPrototypeVector ())->Back();
}

Node* Node::PushNodePrototype(const Node& n)
{ // Add a new node to the top of the prototypes stack
  (*GetPrototypeVector ())->Add(n.Copy());
  return (*GetPrototypeVector ())->Back();
}

Node* Node::PushNodePrototype() 
{ // Replicate the top of the prototype stack
  (*GetPrototypeVector ())->Add(GetNodePrototype()->Copy());
  return (*GetPrototypeVector ())->Back();
}

void Node::PopNodePrototype()
{ 
  if (!(*GetPrototypeVector ())->Empty()) (*GetPrototypeVector ())->Remove();
}

void Node::ClearAll()
{ // Delete all nodes for memory leak checking, including prototypes
  (*GetNodeVector ())->Clear ();
  (*GetPrototypeVector ())->Clear();
}

L3Demux*
Node::GetL3Demux() const
{
  return 0;
}
Ipv4L4Demux*
Node::GetIpv4L4Demux() const
{
  return 0;
}

ApplicationList* Node::GetApplicationList() const
{
  return 0;
}

Ipv4 *
Node::GetIpv4 (void) const
{
  return 0;
}
Udp *
Node::GetUdp (void) const
{
  return 0;
}

Arp *
Node::GetArp (void) const
{
  return 0;
}

}//namespace ns3
