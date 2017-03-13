README - Topology Syntax

A Topology in it's most basic form consists of nodes and links between nodes. The point of connection between a link and a node is an interface. This syntax built on top of the YAML
language relies heavily on the use of interfaces (ifaces) and the ability to "provide" (to have an interface not connected to a link) or to "accept" them (have an unconnected end of
a link).


Let's now look at how to declare simple links and nodes.

#Simple Declarations

A simple, unconnected CSMA link (comments denoted by '#'):
```yaml
link:
    - simpleCsma:           # Name of link I am creating
        type: csma          # type of link (relates to a type of NS-3 link)
        ifacesAccepted:     # declaring that there are unconnected
            - first         # naming the interface acceptors to reference
            - second
```

A simple, unconnected Node with 2 interfaces:
```yaml
node:
    - 2ethNode:             # Name of node
        ifaces:             # Declaring interfaces
            - eth0          # Naming interfaces
            - eth1
```

Now, let's connect two nodes via a link:
```yaml
nodes:                          # Declare nodes
    - 1ethNode1:                # Name of first node
        ifaces:
            - eth0
    - 1ethNode2:                # Name of second node
        ifaces:
            - eth0
link:                           # Declare links
    - nodeLinker:               # Name of link
        type: csma
        ifaces:                 # Connect interfaces to link
            - 1ethNode1 eth0    # Connect 1st node's eth0
            - 1ethNode2 eth0    # Connect 2nd node's eth0
```

#Templating and Inclusion (Includes)
In the previous example, two nearly identical (but differently named) nodes were created. To make it easier to create duplicates of nodes or links, templates can be declared for each.
This is an identical topology to the one previously declared:
```yaml
nodes:                          # Declare nodes
    - 1ethNode:                 # Name of template node
        ifaces:
            - eth0
        num: 2                  # Instantiate TWO nodes of this type, this turns the node into an anonymous template
link:
    - nodeLinker:
        type: csma
        ifaces:                 # Connect interfaces to link
            - 1ethNode_1 eth0   # Reference the instantiated node of a template via '_#', indexing starts at 1
            - 1ethNode_2 eth0   # Connect 2nd node's eth0
```
The previous topology is an example of an anonymous template. Anonymous templates are instantiated within a topology, and cannot be instantiated in another topology directly.

To prevent redundancy between topologies, explicit templates can be declared. Explicit templates can be included from one file into another.
An example of explicit templates.

nodeTemplates.yaml
```yaml
nodeTemplates:
    - 1ethNode:
        ifaces:
            - eth0
```

Now, let's use that template in a topology. To do so, we need to tell the parser we are using another file to help construct our topology. This is done via the "includes"

myTopology.yaml
```yaml
includes:
    - nodeTemplates             # List other files referenced

nodes:                          # Declare nodes
    - myNode:                   # Name of instantiated nodes
        template: 1ethNode      # Which template to instantiate these nodes from
        num: 2                  # Instantiate TWO nodes of this type
link:
    - nodeLinker:
        type: csma
        ifaces:                 # Connect interfaces to link
            - myNode_1 eth0     # Reference the instantiated node of a template via '_#', indexing starts at 1
            - myNode_2 eth0     # Connect 2nd node's eth0
```

The same templating can be done with links.

linkTemplates.yaml
```yaml
linkTemplates:
    - csmaLink:
        type: csma
        ifacesAccepted:
            - first
            - second
```

Now, let's use both of our templates in a topology.

myTopology.yaml
```yaml
includes:
    - nodeTemplates                 # List other files referenced
    - linkTemplates

myTopology:
    nodes:                              # Declare nodes
        - myNode:                       # Name of instantiated nodes
            template: 1ethNode          # Which template to instantiate these nodes from
            num: 2                      # Instantiate TWO nodes of this type
    link:
        - nodeLinker:
            template: csmaLink
            ifaces:                     # Connect interfaces to link
                - first: myNode_1 eth0  # Connect 1st node's eth0 to first ifaceAccepted
                - second: myNode_2 eth0 # Connect 2nd node's eth0 to second ifaceAccepted
```
Notice, this time we explicitly named the topology 'myTopology', this is useful when including topologies in another topology.
Below is a simple topology that will be instantiated/included in another topology:

simpleTop.yaml
```yaml
simpleTop:                          # Name of topology
    nodes:
        - 1ethNode:
            ifaces:
                - eth0
        - 2ethNode:
            ifaces:
                - eth0
                - eth1
    link:
        - nodeLinker:
            type: csma
            ifaces:
                - 1ethNode1 eth0
                - 1ethNode2 eth0
    ifacesProvided:                 # Declare there are unconnected interfaces
        - looseIface:               # Name the unconnected interface
            2ethNode eth1           # Map the name to the actual interface
```

Now, we'll instantiate two of the above topologies, and connect them with a link
```yaml
includes:
    - simpleTop

twoSimpleTops:
    topologies:
        - simpleTopology1:
            template: simpleTop
        - simpleTopology2:
            template: simpleTop
    links:
        - linkThem:
            type: csma
            ifaces:
                - simpleTopology1 looseIface    #Reference the ifaces by what they were named in the ifacesProvided of the subTopology
                - simpleTopology2 looseIface
```

#IP/MAC Address Assignment

#Inheritance