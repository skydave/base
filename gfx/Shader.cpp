#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>






namespace base
{
	Shader::ShaderLoader::ShaderLoader( ShaderPtr shader ) : m_shader(shader)
	{
	}

	Shader::ShaderLoader Shader::ShaderLoader::attach( int shaderType, const std::string &src)
	{
		Shader::ShaderObject &so = m_shader->createShaderObject();
		so.init(shaderType);
		so.compile(src);
		glAttachShader(m_shader->m_glProgram, so.m_id);

		return *this;
	}

	Shader::ShaderLoader Shader::ShaderLoader::attach( int shaderType, Path srcFile )
	{
		Shader::ShaderObject &so = m_shader->createShaderObject();
		so.init(shaderType);
		so.compile(srcFile);
		glAttachShader(m_shader->m_glProgram, so.m_id);
		return *this;
	}

	Shader::ShaderLoader Shader::ShaderLoader::attachPS( Path src )
	{
		return attach( GL_FRAGMENT_SHADER_ARB, src );
	}

	Shader::ShaderLoader Shader::ShaderLoader::attachVS( Path src )
	{
		return attach( GL_VERTEX_SHADER_ARB, src );
	}

	Shader::ShaderLoader Shader::ShaderLoader::attachPS( const std::string &src )
	{
		return attach( GL_FRAGMENT_SHADER_ARB, src );
	}

	Shader::ShaderLoader Shader::ShaderLoader::attachVS( const std::string &src )
	{
		return attach( GL_VERTEX_SHADER_ARB, src );
	}

	Shader::ShaderLoader Shader::ShaderLoader::attachPS( const char *src, const int &srcSize )
	{
		std::string ps_src( src, srcSize );
		return attach( GL_FRAGMENT_SHADER_ARB, ps_src );
	}

	Shader::ShaderLoader Shader::ShaderLoader::attachVS( const char *src, const int &srcSize )
	{
		std::string vs_src( src, srcSize );
		return attach( GL_VERTEX_SHADER_ARB, vs_src );
	}

	Shader::ShaderLoader::operator ShaderPtr()
	{
		m_shader->finalize();

		if( m_shader->isOk() )
			return m_shader;

		return ShaderPtr();
	}


	Shader::ShaderLoader Shader::load( const char *vsSrc, const int &vsSrcSize, const char *psSrc, const int &psSrcSize, const std::string &id )
	{
		return Shader::create( id ).attachVS( vsSrc, vsSrcSize ).attachPS( psSrc, psSrcSize );
	}



	Shader::ShaderObject::ShaderObject()
	{
	}

	void Shader::ShaderObject::init(int shaderType)
	{
		m_shaderType = shaderType;
		m_id = glCreateShader(m_shaderType);
	}

	bool Shader::ShaderObject::compile( Path sourceFile )
	{
		m_source = sourceFile;

		// read vertex shader file content
		// TODO: use filesystem to abstract away fileaccess
		std::string src;
		std::ifstream file(m_source.c_str() );
		if (file.is_open())
		{
			std::stringstream buffer;
			buffer << file.rdbuf();
			src = buffer.str();
			file.close();
		}

		return compile(src);
	}

	bool Shader::ShaderObject::compile( std::string src )
	{
		const char* vsSrcList[1];
		vsSrcList[0] = src.c_str();
		int length = (int) src.size();
		glShaderSource(m_id, 1, (const GLchar **)&vsSrcList, &length);

		glCompileShader(m_id);

		//TODO: error check

		return true;
	}

	bool Shader::ShaderObject::reload()
	{
		if( m_source.IsValid() )
		{
			compile(m_source);
			return true;
		}
		return false;
	}

	void Shader::reload()
	{
		// iterate all shader objects, recompile them
		for( std::vector<ShaderObject>::iterator it = m_objects.begin(); it != m_objects.end(); ++it )
		{
			(*it).reload();
		}

		// then relink
		finalize();
	}

	Shader::ShaderLoader Shader::load( const std::string &vertexShaderPath, const std::string &pixelShaderPath, const std::string &id )
	{
		std::string vsSrc, psSrc;

		{
			// read vertex shader file content
			std::ifstream vsFile(vertexShaderPath.c_str() );
			if (vsFile.is_open())
			{
				std::stringstream buffer;
				buffer << vsFile.rdbuf();
				vsSrc = buffer.str();
				vsFile.close();
			}
			else
			{
				std::cout << "Unable to open vertexshader file\n";
				return Shader::ShaderLoader(ShaderPtr());
			}
		}

		{
			// read pixel shader file content
			std::ifstream psFile(pixelShaderPath.c_str());
			if (psFile.is_open())
			{
				std::stringstream buffer;
				buffer << psFile.rdbuf();
				psSrc = buffer.str();
				psFile.close();
			}
			else
			{
				std::cout << "Unable to open pixelshader file\n";
				return Shader::ShaderLoader(ShaderPtr());
			}
		}

		return Shader::create( id ).attachVS( vsSrc ).attachPS( psSrc );
	}

	Shader::ShaderLoader Shader::load( Path vertexShaderPath, Path pixelShaderPath, const std::string &id )
	{
		return Shader::create( id ).attach( GL_VERTEX_SHADER_ARB, vertexShaderPath ).attach( GL_FRAGMENT_SHADER_ARB, pixelShaderPath );
	}


	Shader::ShaderLoader Shader::create( const std::string &id )
	{
		ShaderPtr shader = ShaderPtr( new Shader() );
		return Shader::ShaderLoader( shader );
	}

	Shader::ShaderObject &Shader::createShaderObject()
	{
		m_objects.push_back( Shader::ShaderObject() );
		return m_objects.back();
	}

	Shader::Shader() : m_isOk(false)
	{
		m_glProgram = glCreateProgram();
	}

	bool Shader::isOk()
	{
		return m_isOk;
	}





	void Shader::finalize()
	{
		char text[1000];
		//
		// bind attribute locations
		//
		glBindAttribLocation(m_glProgram, 0, "P");
		glBindAttribLocation(m_glProgram, 1, "N");
		glBindAttribLocation(m_glProgram, 2, "Cd");
		glBindAttribLocation(m_glProgram, 3, "W");
		glBindAttribLocation(m_glProgram, 4, "CMT");
		glBindAttribLocation(m_glProgram, 5, "BW");
		glBindAttribLocation(m_glProgram, 6, "BI");
		glBindAttribLocation(m_glProgram, 7, "UV");

		//
		// program linking --------
		//
		glLinkProgram(m_glProgram);
		glGetInfoLogARB(m_glProgram, 1000, 0, text);
		std::cout << "Shader::finalize:\n" << text << std::endl;


		// extract active attributes info
		int numActiveAttributes = 0;
		m_activeAttributes.clear();
		glGetProgramiv(m_glProgram, GL_ACTIVE_ATTRIBUTES, &numActiveAttributes);
		std::cout << "number of active attributes: " << numActiveAttributes << std::endl;
		for( int i=0;i<numActiveAttributes; ++i )
		{
			char name[1000];
			int length;
			int size;
			unsigned int type;
			glGetActiveAttrib( m_glProgram, i, 1000, &length, &size, &type, name );
			int index = glGetAttribLocation(m_glProgram, name);
			std::cout << "active attribute " << name << " at location " << index << std::endl;
			m_activeAttributes.insert(std::make_pair( std::string(name), index ));
		}

		// extract active uniforms info
		int numActiveUniforms = 0;
		m_activeUniforms.clear();
		glGetProgramiv(m_glProgram, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
		std::cout << "number of active uniforms: " << numActiveUniforms << std::endl;
		for( int i=0;i<numActiveUniforms; ++i )
		{
			char name[1000];
			int length;
			int size;
			unsigned int type;
			glGetActiveUniform( m_glProgram, i, 1000, &length, &size, &type, name );
			int index = glGetUniformLocation(m_glProgram, name);
			std::cout << "active uniform " << name << " at location " << index << std::endl;

			// index==-1 means uniform is a built in uniform and we dont touch it
			if( index != -1 )
			{

				// fucking ati laptop cards puts an [i] at the end of array uniforms. have
				// to remove that to remain compatible with the other shit
				int l = (int)strlen( name );
				if( (name[l-3] == '[') && (name[l-2] == '0') && (name[l-1] == ']') )
					name[l-3] = '\0';

				m_activeUniforms.insert( std::make_pair( std::string(name), index) );
			}
		}

		m_isOk = true;
	}



	void Shader::setUniform( const std::string &name, AttributePtr uniform )
	{
		m_uniforms[name] = uniform;
	}

	void Shader::setUniform( const std::string &name, float v0, float v1, float v2, float v3 )
	{
		AttributePtr u = getUniform(name);
		if(!u)
		{
			u = Attribute::createVec4f();
			u->appendElement<float>(v0,v1,v2,v3);
			setUniform(name, u);
		}else
			u->set<float>(0, v0, v1, v2, v3);
	}

	void Shader::setUniform( const std::string &name, math::Vec3f value )
	{
		AttributePtr u = getUniform(name);
		if(!u)
		{
			u = Attribute::createVec3f();
			u->appendElement<math::Vec3f>(value);
			setUniform(name, u);
		}else
			u->set<math::Vec3f>(0, value);
	}

	void Shader::setUniform( const std::string &name, math::Matrix44f value )
	{
		AttributePtr u = getUniform(name);
		if(!u)
		{
			u = Attribute::createMat44();
			u->appendElement<math::Matrix44f>(value);
			setUniform(name, u);
		}else
			u->set<math::Matrix44f>(0, value);
	}

	void Shader::setUniform( const std::string &name, float value )
	{
		AttributePtr u = getUniform(name);
		if(!u)
		{
			u = Attribute::createFloat();
			u->appendElement<float>(value);
			setUniform(name, u);
		}else
			u->set<float>(0, value );
	}

	void Shader::setUniform( const std::string &name, int value )
	{
		AttributePtr u = getUniform(name);
		if(!u)
		{
			u = Attribute::createInt();
			u->appendElement<int>(value);
			setUniform(name, u);
		}else
			u->set<int>(0, value);
	}

	bool Shader::hasUniform( const std::string &name )
	{
		return (m_uniforms.find( name ) != m_uniforms.end());
	}

	AttributePtr Shader::getUniform( const std::string &name )
	{
		std::map<std::string, AttributePtr>::iterator it = m_uniforms.find( name );
		if(it != m_uniforms.end())
			return it->second;
		return AttributePtr();
	}

}



