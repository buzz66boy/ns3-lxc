# README - Topology Syntax

A Topology in it's most basic form consists of nodes and links between nodes. The point of connection between a link and a node is an interface. This syntax built on top of the YAML
language relies heavily on the use of interfaces (ifaces) and the ability to "provide" (to have an interface not connected to a link) or to "accept" them (have an unconnected end of
a link).


Let's now look at how to declare simple links and nodes.

## Simple Declarations

A simple, unconnected Node with 2 interfaces:
```yaml
node:
    - 2ethNode:             # Name of node
        ifaces:             # Declaring interfaces
            - eth0          # Naming interfaces
            - eth1
```

A simple, unconnected CSMA link (comments denoted by '#'):
```yaml
link:
    - simpleCsma:           # Name of link I am creating
        type: csma          # type of link (relates to a type of NS-3 link)
        ifacesAccepted:     # declaring that there are unconnected 
            - first         # naming the interface acceptors to reference
            - second
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
            - 1ethNode1 eth0 10.0.0.1   # Connect 1st node's eth0 and assign ip
            - 1ethNode2 eth0 10.0.0.2   # Connect 2nd node's eth0 and assign ip
```

## Templating and Inclusion (Includes)
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
            - 1ethNode_1 eth0 10.0.0.1  # Reference the instantiated node of a template via '_#', indexing starts at 1
            - 1ethNode_2 eth0 10.0.0.2  # Connect 2nd node's eth0
```
The previous topology is an example of an anonymous template. Anonymous templates are instantiated within a topology, and cannot be instantiated in another topology directly.

To prevent redundancy between topologies, explicit templates can be declared. Explicit templates can be included from one file into another.
An example of explicit templates.

### nodeTemplates.yaml
```yaml
nodeTemplates:
    - 1ethNode:
        ifaces:
            - eth0
```

Now, let's use that template in a topology. To do so, we need to tell the parser we are using another file to help construct our topology. This is done via the "includes"

### myTopology.yaml
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
            - myNode_1 eth0 10.0.0.1    # Reference the instantiated node of a template via '_#', indexing starts at 1
            - myNode_2 eth0 10.0.0.2    # Connect 2nd node's eth0
```

The same templating can be done with links.

### linkTemplates.yaml
```yaml
linkTemplates:
    - csmaLink:
        type: csma
        ifacesAccepted:
            - first
            - second
```

Now, let's use both of our templates in a topology.

### myTopology.yaml
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
                - first: myNode_1 eth0 10.0.0.1  # Connect 1st node's eth0 to first ifaceAccepted
                - second: myNode_2 eth0 10.0.0.2 # Connect 2nd node's eth0 to second ifaceAccepted
```
Notice, this time we explicitly named the topology 'myTopology', this is useful when including topologies in another topology.
Below is a simple topology that will be instantiated/included in another topology:

### simpleTop.yaml
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
                - 1ethNode eth0 10.0.0.1
                - 2ethNode eth0 10.0.0.2
    ifacesProvided:                 # Declare there are unconnected interfaces
        - looseIface:               # Name the unconnected interface
            2ethNode eth1           # Map the name to the actual interface
```

Now, we'll instantiate two of the above topologies, and connect them with a link
### twoSimpleTops.yaml
```yaml
includes:
    - simpleTop

twoSimpleTops:
    topologies:
        - simpleTopology:                      #Instantiate two instances of simpleTop
            template: simpleTop
            num: 2
    links:
        - linkThem:
            type: csma
            ifaces:
                - simpleTopology_1 looseIface 11.0.0.1   #Reference the ifaces by what they were named in the ifacesProvided of the subTopology
                - simpleTopology_2 looseIface 11.0.0.2
```

## Inheritance

When using templates for nodes, links, or topologies, an important factor to understand is the use of inheritance and how it applies to those structures.

In the scope of this project, inheritance refers to the sharing of attributes between a template and it's child element. In much the same way as Object-Oriented
Programming, child element's declared attributes should override the attributes of the template (NOTE: the ability to override attributes is only partially supported as of 25 May '17). 

## IP/MAC Address Assignment

IPs are assigned to interfaces when they are connected to an IfaceAcceptor/Link


## Positions and Rotations