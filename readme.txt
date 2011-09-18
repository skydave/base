

-wir brauchen extrem simples operator framework fuer:
+procedural mesh generation (sphere fuer demo test)
+procedural texture generation (generally usefull)
+expressing rendering as operator graph (fbo, gpu based image processing etc. -> for ssao)
->requires support for multiple outputs (multirendertarget)
+gpu based particle system graph
+channel operator graph (required for best application of sync data from processing)
->requires lazy evaluation since we dont want to presample the channels

concept
-------
-base/gfx/Object.h
-base/gfx/Mesh.h (look at cinder, openframework, etc.)
-base/op/Op.h
->status execute()
->std::vector<OperatorPtr> m_inputs
->schreibt in std::vector<ObjectPtr> m_outputs;
-base/op/OpGroup.h //derives from op
->maintains list of all ops it contains
->status execute(  ) from operator
->->starts at finalOp and traverses the tree. fully evaluates all input ops
->input from parent group passed using GroupInputOp (similar to houdini's for each vop)
->has a finalOp which provides the result of the group
->status connect( src, dst ) // appends src to inputs of dst


stage1:
SphereOp to create a Mesh
Mesh::getGeometry() to turn it into a renderable
and render it

stage2:
RenderOp (group)
ClearOp
RenderMeshOp to display a mesh on screen














-sound:
portaudio
http://nothings.org/stb_vorbis/










TODO:




-figure out how to handle forward declaration. compiletime will increase if we dont do something here
-Attribute bind function uploads all data every time - should only be done when something has changed
-context render uploads the whole index buffer everytime geometry is rendered - should be done only when index buffer changes
-attribute bind as uniform, when being a sampler used uniformindex as texture unit to bind to - very bad - need textureunit management






serialisation
=============

soon we will need a way to serialise objects. For example after editing our clouds effect we want to save it and later load it with the saved settings.


use case: cloud effect
-the cloud effect is basically a shader
-
-edit clouds effect values using ui -> this changes class members and attributes(uniforms) which are passed to shader
-serialise the clouds effect the data is burried within the attributes (uniforms)


-ideally: no << or >> operator overloading and serialising to stream
-I like goggles protocoll buffers (map of key value pairs). because their approach allows smoother interoperability between different versions



clouds
------
node:
the clouds node is basically a wrapper around the cloudshader. it also contains a texture (clouds_parameters) and other things which are updated when inputs change.

inputs:
the glsl code has a lots of uniforms and attributes of which some of them are used internally (clouds_parameters texture) but others might be exposed.

outputs:
the wrapped glsl shader will be the output of the clouds effect.

notes:
update is implicitly carried out when shader is bound. at that time the render code iterates over the uniforms/attributes of the shader and updates them on the gpu.


render geometry
---------------

node:
this node just does a context->render( shader, geometry )

inputs:
base::Geometry
base::Shader

outputs:
execute function

note:
no update has to be carried out since the execute function passed will be used when needed by downstream nodes


ssao
----
node:
is basically a group node which contains a subgraph.

inputs:
pos/normal buffer texture
depth buffer texture

outputs:
color buffer texture



distilled ideas:
================

-nodes should be able to pass any of their members (data or methods) to other nodes
-if 2 references are connected a datacontainer is created inbetween the connection and given to both
	todo: how to handle traits?
	maybe outputsockets can have traits associated with them
