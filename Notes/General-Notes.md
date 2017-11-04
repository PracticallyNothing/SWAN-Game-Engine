---------------
# RenderQueue

### Why?
In [Commit 47efc12](https://github.com/PracticallyNothing/SWAN-Game-Engine/tree/47efc12cfd1230d5d2becfe84ffeb0de6097f1c6), Tooltips were added. However, there is a problem with them.  

***They didn't render correctly.***

More specifically, because of the way they are set up, they render before
the other GUI elements.  

That means that whenever a tooltip is rendered, everything else will be rendered *above it*, thus obscuring it.

A render queue would very easily fix this problem.

### How will it help?
- **[CURRENTLY]** Sort the list of render objects (transparency, GUI after 3D, etc.)
- Abstract away calls to a rendering API (OpenGL, DirectX, Vulkan, etc.)

### How will it work?
#### In theory:

The render queue would store bundles of render commands.  

Then, when a render needed to happen, the queue would pop the bundles one by one and execute all of the commands in each.  

After the render was complete, the queue would be completely empty and every object that wanted to be rendered would have to insert its command bundle again.

#### In practice:

This could be implemented in a multitude of ways, but special attention needs to be payed to:
  - cheap insertions and deletions of command bundles
  - compact command bundles  

The first can easily be fullfilled by a `std::deque`. However, the second will be tricky. The command bundles need to be compact so that the least amount of overhead possible is achieved.  

**Currently**, since only GL 3.3 and up are supported, this can be implemented in terms of calls to SWAN::Shader member functions, namely `setUniforms()` and `renderMesh()`.

---------------
# Resource tickets

### Why?
Currently, whenever a `Mesh` or `Shader` has to be initialized, a call to OpenGL must be made.  

However, not always is there a guarantee that an OpenGL context will be ready when the object is being created (*e.g. when statically initializing a `Mesh` or `Shader`*).

Further adding to the confusion, the call that actually initializes the OpenGL context is `Display::Init()`.

Adding a Resource ticket system would solve this problem.

### How will it help?
- **[CURRENTLY]** Eliminate unexplained segfaults when statically initializing an object that relies on OpenGL.
- Limit the amount of file parsing that needs to be done.
- Add a reference count for resources

### How will it work?
The structure that will hold the tickets can be made as simple as this:   
`std::vector<ResourceTicket> tickets;`  

The tricky part is the actually the `ResourceTicket` structure.
It needs to store four things:
- the time when the ticket was filed,
- the type of resource that's going to be loaded
- where the resource will be loaded from
- how the resource will be referenced in future

Something like the following:
```cpp
struct ResourceTicket {
	TimePointT timeFiled; // Stores the time when the ticket was filed.

	enum Type {
		T_NONE,    // Empty/invalid ticket
		T_MESH,    // Ticket for a Mesh
		T_SHADER,  // Ticket for a Shader
		T_TEXTURE, // Ticket for a Texture
		//...
	} type; // Stores the type of the ticket (e.g. Mesh, Texture, Shader, etc.)

	std::string
		file, // File of the resource
		name; // Name of the resource

	int priority = 1; // Priority management
};
```

Once the time comes for the tickets to be fullfilled, the process goes as follows:
1. Go over the array while counting and storing the unique tickets.  Two tickets are considered the same if both their file and type match. Every time two versions of the same request show up, add one to the priority of the unique element.
2. Sort the array by priority.
3. Fulfill all of the tickets in the sorted order.

---
