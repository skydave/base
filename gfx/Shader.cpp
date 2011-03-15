#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>






namespace base
{
	ShaderPtr Shader::load( const std::string &vertexShaderPath, const std::string &pixelShaderPath )
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
				return ShaderPtr();
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
				return ShaderPtr();
			}
		}
		
		ShaderPtr shader = ShaderPtr( new Shader() );
		shader->init( vsSrc, psSrc, vertexShaderPath+"/"+pixelShaderPath );
		shader->finalize();

		if( shader->isOk() )
			return shader;

		return ShaderPtr();
	}


	Shader::Shader() : m_isOk(false)
	{
	}

	bool Shader::isOk()
	{
		return m_isOk;
	}

	void Shader::init(const std::string &vsSrc, const std::string &psSrc, const std::string &id)
	{
		std::cout << "Shader::init " << id << std::endl;

		m_glProgram = glCreateProgram();

		// vertex shader ------
		attach(GL_VERTEX_SHADER_ARB, vsSrc, "vertexShader");

		// pixel shader ------
		attach(GL_FRAGMENT_SHADER_ARB, psSrc, "pixelShader");

	}


	void Shader::attach(int shaderType, const std::string &src, const std::string &id )
	{
		//char text[1000];

		GLhandleARB s = glCreateShader(shaderType);
		const char* vsSrcList[1];
		vsSrcList[0] = src.c_str();
		int length = (int) src.size();
		glShaderSource(s, 1, (const GLchar **)&vsSrcList, &length);
		glCompileShader(s);

		//glGetInfoLogARB(s, 1000, 0, text);
		//std::cout << "Shader::attach log "<<id<<":\n" << text << std::endl;

		glAttachShader(m_glProgram, s);
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



		/*
	void Shader::use()
	{
		glUseProgram(m_glProgram);

		// iterate all active uniforms
		for( int i=0; i<m_activeUniforms.size(); ++i )
		{
			// iterate over all global uniforms and bind it if we found one
			for( int j=0; j<g_globalUniforms.size(); ++j )
			{
				char *t1 = (char *)m_activeUniformNames.m_data[i];
				char *t2 = (char *)g_globalUniformNames.m_data[j];
				if( !strcmp( (char *)m_activeUniformNames.m_data[i], g_globalUniformNames.m_data[j] ) )
				{
					((Attribute*)(g_globalUniforms.m_data[j]))->bindAsUniform( m_activeUniforms.m_data[i] );
					break;
				}
			}
			// now iterate over all local uniforms and bind the ones we found
			for( int j=0; j<m_uniforms.size(); ++j )
			{
				char *t1 = (char *)m_activeUniformNames.m_data[i];
				const char *t2 = m_uniformNames.m_data[j];
				if( !strcmp( (char *)m_activeUniformNames.m_data[i], m_uniformNames.m_data[j] ) )
				{
					((Attribute*)(m_uniforms.m_data[j]))->bindAsUniform( m_activeUniforms.m_data[i] );
					break;
				}
			}
		}

	}
		*/

	void Shader::setUniform( const std::string &name, AttributePtr uniform )
	{
		m_uniforms[name] = uniform;
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








/*






int atexit ( void ( * function ) (void) )
{
	return 0;
}


vector<void *> Shader::g_globalUniforms; // list of uniforms
vector<const char *> Shader::g_globalUniformNames; // list of uniform names

void Shader::setGlobalUniform( const char *name, Attribute *uniform )
{
	g_globalUniformNames.push_back( name );
	g_globalUniforms.push_back( uniform );
}



Attribute *Shader::getUniform( int uniformIndex )
{
	return (Attribute *)m_uniforms.m_data[uniformIndex];
}

void Shader::setUniform( const char *name, Attribute *uniform )
{
	// for each registered uniform
	for( int i=0; i<m_uniformNames.size(); ++i )
	{
		// if uniform already has been registered under that name
		if( !strcmp( (char *)m_uniformNames.m_data[i], name ) )
		{
			// just change the reference
			m_uniforms.m_data[i] = uniform;
			// done
			return;
		}
	}

	// no uniform existed add it
	m_uniformNames.push_back( name );
	m_uniforms.push_back( uniform );
}
*/
