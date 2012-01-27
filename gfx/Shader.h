#pragma once
#include <string>
#include <vector>
#include <map>
#include <util/shared_ptr.h>
#include <util/Path.h>
#include <math/Vec3f.h>

#include <gltools/gl.h>
#include "Attribute.h"

#include <iostream>

namespace base
{

	BASE_DECL_SMARTPTR_STRUCT(Shader);

	struct Shader
	{
		struct ShaderLoader
		{
			ShaderLoader( ShaderPtr shader );

			ShaderLoader attach( int shaderType, const std::string &src);
			ShaderLoader attach( int shaderType, Path srcFile );
			ShaderLoader attachPS( const std::string &src);
			ShaderLoader attachVS( const std::string &src);
			ShaderLoader attachPS( Path src);
			ShaderLoader attachVS( Path src);
			ShaderLoader attachPS( const char *src, const int &srcSize );
			ShaderLoader attachVS( const char *src, const int &srcSize );

			operator ShaderPtr ();
			ShaderPtr m_shader;
		};
		struct ShaderObject
		{
			ShaderObject();
			void init(int shaderType);
			bool compile( Path sourceFile );
			bool compile( std::string src );
			bool reload();
			Path m_source;
			int m_shaderType;
			GLhandleARB m_id;
		};


		Shader();

		//void init(const std::string &vsSrc, const std::string &psSrc, const std::string &id = "");
		bool isOk();
		void finalize();



		// set uniforms
		// set vertex attributes
		// list of active attributes (which are required by this shader) (name mapped to index)
		std::map<std::string, int> m_activeAttributes;
		// list of active uniforms (which are required by this shader) (name mapped to index)
		std::map<std::string, int> m_activeUniforms;


		//
		// local uniform management
		//
		void                      setUniform( const std::string &name, AttributePtr uniform );
		void                         setUniform( const std::string &name, math::Vec3f value ); // convinience function
		void                     setUniform( const std::string &name, math::Matrix44f value ); // convinience function
		void                               setUniform( const std::string &name, float value ); // convinience function
		void                                 setUniform( const std::string &name, int value ); // convinience function
		void    setUniform( const std::string &name, float v0, float v1, float v2, float v3 ); // convinience function
		bool                                            hasUniform( const std::string &name );
		AttributePtr                                    getUniform( const std::string &name );
		std::map<std::string, AttributePtr>                                        m_uniforms; // list of uniforms


		//
		// creation functions
		//
		static ShaderLoader create( const std::string &id = "" );
		static ShaderLoader load( const std::string &vertexShaderPath, const std::string &pixelShaderPath, const std::string &id = "" );
		static ShaderLoader load( const char *vsSrc, const int &vsSrcSize, const char *psSrc, const int &psSrcSize, const std::string &id = "" );
		static ShaderLoader load( Path vertexShaderPath, Path pixelShaderPath, const std::string &id = "" );

		// list of glshaders which are linked into the glprogram
		ShaderObject &createShaderObject();
		std::vector<ShaderObject> m_objects;
		void reload();

		GLhandleARB m_glProgram;
		bool             m_isOk;
	};




}
