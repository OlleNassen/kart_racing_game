static GLuint CompileShader(char *ShaderCode, GLuint ShaderType)
{
    GLuint ShaderID = glCreateShader(ShaderType);
    glShaderSource(ShaderID, 1, &ShaderCode, 0);
    glCompileShader(ShaderID);
    
    int Successful;
    char ErrorLog[512];
    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Successful);
    if (!Successful)
    {
        glGetShaderInfoLog(ShaderID, 512, 0, ErrorLog);
        char* ShaderTypeString;
        switch(ShaderType)
        {
            case GL_VERTEX_SHADER:
            ShaderTypeString = "Vertex Shader";
            break;
            case GL_FRAGMENT_SHADER:
            ShaderTypeString = "Fragment Shader";
            break;
            default:
            ShaderTypeString = "Unknown Shader Type";
            break;
        }
        printf("Shader failed to compile: %s\n%s\n\n",ShaderTypeString, ErrorLog);
    }
    return ShaderID;
}

GLuint LoadAndCreateShaders(game_state* GameState, char *VertexShaderPath, char *FragmentShaderPath)
{
    char VertexBuffer[10000] = {0};
    
    FILE *VertexFile;
    VertexFile = fopen(VertexShaderPath, "r");
    if(!VertexFile)
        printf("Failed to open path: %s", VertexShaderPath);
    
    fseek (VertexFile , 0 , SEEK_END);
    long VertexFileSize = ftell(VertexFile);
    rewind (VertexFile);
    
    fread(VertexBuffer, 1, VertexFileSize, VertexFile);
    GLuint CompiledVertex = CompileShader((char*)VertexBuffer, GL_VERTEX_SHADER);
    fclose(VertexFile);
    
    char FragmentBuffer[10000] = {0};
    
    FILE *FragmentFile = fopen(FragmentShaderPath, "r");
    if(!VertexFile)
        printf("Failed to open path: %s", FragmentShaderPath);
    
    fseek(FragmentFile, 0, SEEK_END);
    long FragmentFileSize = ftell(FragmentFile);
    fseek(FragmentFile, 0, SEEK_SET);
    
    fread(FragmentBuffer, 1, FragmentFileSize, FragmentFile);
    GLuint CompiledFragment = CompileShader((char*)FragmentBuffer, GL_FRAGMENT_SHADER);
    fclose(FragmentFile);
    
    GLuint ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, CompiledVertex);
    glAttachShader(ShaderProgram, CompiledFragment);
    glLinkProgram(ShaderProgram);
    
    s32 Successful;
    char ErrorLog[512];
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Successful);
    if (!Successful)
    {
        glGetProgramInfoLog(ShaderProgram, 512, 0, ErrorLog);
        printf("ERROR::PROGRAM_LINKING_ERROR of type: PROGRAM\n%s\n\n", ErrorLog);
    }
    
    glDeleteShader(CompiledVertex);
    glDeleteShader(CompiledFragment);
    
    return ShaderProgram;
}