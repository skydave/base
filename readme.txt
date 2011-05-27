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
