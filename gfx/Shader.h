#pragma once
#include <string>
#include <vector>
#include <map>
#include <util/shared_ptr.h>

#include <gltools/gl.h>

namespace base
{
	//struct Attribute;

	BASE_DECL_SMARTPTR(Shader);

	struct Shader
	{
		Shader();

		void init(const std::string &vsSrc, const std::string &psSrc, const std::string &id = "");
		bool isOk();
		void attach(int shaderType, const std::string &src, const std::string &id = "");
		void finalize();
		void use();



		// set uniforms
		// set vertex attributes
		// list of active attributes (which are required by this shader) (name mapped to index)
		std::map<std::string, int> m_activeAttributes;
		// list of active uniforms (which are required by this shader) (name mapped to index)
		std::map<std::string, int> m_activeUniforms;


		//
		// local uniform management
		//
		/*
		void setUniform( const char *name, Attribute *uniform );
		Attribute               *getUniform( int uniformIndex );
		vector<void *>                               m_uniforms; // list of uniforms
		vector<const char *>                     m_uniformNames; // list of uniform names
		*/
		//
		// global uniform management
		// global uniforms are attributes which will be used by all shaders
		// examples are: mvp, permutationTable, etc.
		//
		/*
		static void setGlobalUniform( const char *name, Attribute *uniform );
		static vector<void *> g_globalUniforms; // list of uniforms
		static vector<const char *> g_globalUniformNames; // list of uniform names
		*/

		//
		// creation functions
		//
		static ShaderPtr load( const std::string &vertexShaderPath, const std::string &pixelShaderPath );

		GLhandleARB m_glProgram;
		bool             m_isOk;
	};




}