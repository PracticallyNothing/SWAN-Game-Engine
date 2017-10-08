---------------------------------------------

Currently in SWAN, if a mesh has to be rendered, the following code has to be
used:
```cpp
SWAN::Mesh someMesh;
SWAN::Shader someShader;

// Initialize someShader...

someShader.use(); // Bind the Shader so that uniforms can be set
	// Set some uniforms to be used inside the shader.
	someShader.setUniformData("name", data);
	someShader.setUniformData("name", data);
	someShader.setUniformData("name", data);
	someShader.setUniformData("name", data);
	someShader.setUniformData("name", data);
	someShader.setUniformData("name", data);
	//...
	someMesh.render(); // Render a mesh
someShader.unuse(); // Disable the shader so that others can be used
```

While the syntax is ok for a few uniforms, it quickly gets cumbersome.  
There is the possibility of forgetting to write `shader.use()` and `shader.unuse()`.  
The syntax also doesn't follow the coding conventions, since the mesh has
a `render` function even though the shader is what is doing the rendering.

---------------------------------------------

A preffered syntax for this would look like this:
```cpp
SWAN::Shader someShader;
SWAN::Mesh someMesh;

// Initialize someShader...

vector<pair<
	string,
	struct { int type; union { int i; float f; /*...*/ } data; }
>> uniforms = {
	{"name", {INT,   data}},
	{"name", {FLOAT, data}},
	{"name", {MAT4,  data}},
	//...
};

someShader.setUniforms(uniforms);  // Set uniforms based on vector
someShader.render(someMesh); // Render mesh
```

This new syntax offers more safety, convenience and better adherence to the coding
conventions.

---------------------------------------------
