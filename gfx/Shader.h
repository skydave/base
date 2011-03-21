#pragma once
#include <string>
#include <vector>
#include <map>
#include <util/shared_ptr.h>

#include <gltools/gl.h>

#include <iostream>

namespace base
{
	//struct Attribute;

	BASE_DECL_SMARTPTR(Shader);

	struct Shader
	{
		struct ShaderLoader
		{
			ShaderLoader( ShaderPtr shader );

			ShaderLoader attach( int shaderType, const std::string &src);
			ShaderLoader attachPS( const std::string &src);
			ShaderLoader attachVS( const std::string &src);
			ShaderLoader attachPS( const char *src, const int &srcSize );
			ShaderLoader attachVS( const char *src, const int &srcSize );

			operator ShaderPtr ();
			ShaderPtr m_shader;
		};


		Shader();

		//void init(const std::string &vsSrc, const std::string &psSrc, const std::string &id = "");
		bool isOk();
		void finalize();
		void use();



		// set uniforms
		// set vertex attributes
		// list of active attributes (which are required by this shader) (name mapped to index)
		std::map<std::string, int> m_activeAttributes;
		// list of active uniforms (which are required by this shader) (name mapped to index)
		std::map<std::string, int> m_activeUniforms;



		//
		// creation functions
		//
		static ShaderLoader create( const std::string &id = "" );
		static ShaderLoader load( const std::string &vertexShaderPath, const std::string &pixelShaderPath, const std::string &id = "" );
		static ShaderLoader load( const char *vsSrc, const int &vsSrcSize, const char *psSrc, const int &psSrcSize, const std::string &id = "" );

		GLhandleARB m_glProgram;
		bool             m_isOk;
	};




}
