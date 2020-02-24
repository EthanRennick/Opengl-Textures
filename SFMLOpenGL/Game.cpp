#include <Game.h>

static bool flip;

Game::Game() : window(VideoMode(800, 600), "OpenGL Cube Texturing")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[4];
	float texel[2];
} Vertex;

Vertex vertex[24];
GLubyte triangles[36];
vector<gpp::Vector3> cube;
vector<gpp::Vector3> aaaa;

/* Variable to hold the VBO identifier and shader data */
GLuint	index,		//Index to draw
		vsid,		//Vertex Shader ID
		fsid,		//Fragment Shader ID
		progID,		//Program ID
		vao = 0,	//Vertex Array ID
		vbo[1],		// Vertex Buffer ID
		positionID, //Position ID
		colorID,	// Color ID
		to,			// Texture ID 1 to 32
		textureID,	//Texture ID
		texelID;	// Texel ID

//const string filename = "texture.tga";
//const string filename = "cube.tga";

const string filename = "cube.tga";

int width; //width of texture
int height; //height of texture
int comp_count; //Component of texture
const int number = 4; //4 = RGBA

unsigned char* img_data;

void Game::initialize()
{
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;
	glEnable(GL_CULL_FACE);

	glewInit();
	
	cube.push_back(gpp::Vector3(-1.0f, 1.0f, 1.0f)); //2 - 0
	cube.push_back(gpp::Vector3(-1.0f, -1.0f, 1.0f)); //1 - 1
	cube.push_back(gpp::Vector3(1.0f, -1.0f, 1.0f)); //0 - 2
	cube.push_back(gpp::Vector3(1.0f, 1.0f, 1.0f)); //3 - 3
	cube.push_back(gpp::Vector3(-1.0f, 1.0f, -1.0f)); //6 - 4
	cube.push_back(gpp::Vector3(1.0f, 1.0f, -1.0f)); //7 - 5
	cube.push_back(gpp::Vector3(1.0f, -1.0f, -1.0f)); //5 - 6
	cube.push_back(gpp::Vector3(-1.0f, -1.0f, -1.0f)); //4 - 7

	DEBUG_MSG(glGetString(GL_VENDOR));
	DEBUG_MSG(glGetString(GL_RENDERER));
	DEBUG_MSG(glGetString(GL_VERSION));


	// Front Face
	/* Vertices counter-clockwise winding */
	vertex[0].coordinate[0] = (cube.at(0).x); //3
	vertex[0].coordinate[1] = (cube.at(0).y); //3
	vertex[0].coordinate[2] = (cube.at(0).z); //3
	vertex[1].coordinate[0] = (cube.at(1).x); //1
	vertex[1].coordinate[1] = (cube.at(1).y); //1
	vertex[1].coordinate[2] = (cube.at(1).z); //1
	vertex[2].coordinate[0] = (cube.at(2).x); //2
	vertex[2].coordinate[1] = (cube.at(2).y); //2
	vertex[2].coordinate[2] = (cube.at(2).z); //2
	vertex[3].coordinate[0] = (cube.at(3).x); //3
	vertex[3].coordinate[1] = (cube.at(3).y); //3
	vertex[3].coordinate[2] = (cube.at(3).z); //3

	// Back Face
	vertex[4].coordinate[0] = (cube.at(4).x); //1
	vertex[4].coordinate[1] = (cube.at(4).y); //1
	vertex[4].coordinate[2] = (cube.at(4).z); //1
	vertex[5].coordinate[0] = (cube.at(5).x); //2
	vertex[5].coordinate[1] = (cube.at(5).y); //2
	vertex[5].coordinate[2] = (cube.at(5).z); //2
	vertex[6].coordinate[0] = (cube.at(6).x); //1
	vertex[6].coordinate[1] = (cube.at(6).y); //1
	vertex[6].coordinate[2] = (cube.at(6).z); //1
	vertex[7].coordinate[0] = (cube.at(7).x); //2
	vertex[7].coordinate[1] = (cube.at(7).y); //2
	vertex[7].coordinate[2] = (cube.at(7).z); //2

	// Right Face

	vertex[8].coordinate[0] = (cube.at(3).x); //
	vertex[8].coordinate[1] = (cube.at(3).y); //
	vertex[8].coordinate[2] = (cube.at(3).z); //
	vertex[9].coordinate[0] = (cube.at(5).x); //
	vertex[5].coordinate[1] = (cube.at(5).y); //
	vertex[9].coordinate[2] = (cube.at(5).z); //
	vertex[10].coordinate[0] = (cube.at(6).x); //
	vertex[10].coordinate[1] = (cube.at(6).y); 
	vertex[10].coordinate[2] = (cube.at(6).z); //
	vertex[11].coordinate[0] = (cube.at(2).x); //
	vertex[11].coordinate[1] = (cube.at(2).y); //
	vertex[11].coordinate[2] = (cube.at(2).z); //

	//left
	vertex[12].coordinate[0] = (cube.at(4).x); //
	vertex[12].coordinate[1] = (cube.at(4).y); //
	vertex[12].coordinate[2] = (cube.at(4).z); //

	vertex[13].coordinate[0] = (cube.at(7).x); //
	vertex[13].coordinate[1] = (cube.at(7).y); //
	vertex[13].coordinate[2] = (cube.at(7).z); //

	vertex[14].coordinate[0] = (cube.at(1).x); //
	vertex[14].coordinate[1] = (cube.at(1).y); //
	vertex[14].coordinate[2] = (cube.at(1).z); //

	vertex[15].coordinate[0] = (cube.at(0).x); //
	vertex[15].coordinate[1] = (cube.at(0).y); //
	vertex[15].coordinate[2] = (cube.at(0).z); //
	//0471
	vertex[0].color[0] = 1.0f;
	vertex[0].color[1] = 0.0f;
	vertex[0].color[2] = 0.0f;
	vertex[0].color[3] = 1.0f;

	vertex[1].color[0] = 1.0f;
	vertex[1].color[1] = 0.0f;
	vertex[1].color[2] = 0.0f;
	vertex[1].color[3] = 1.0f;

	vertex[2].color[0] = 1.0f;
	vertex[2].color[1] = 0.0f;
	vertex[2].color[2] = 0.0f;
	vertex[2].color[3] = 0.0f;

	vertex[3].color[0] = 1.0f;
	vertex[3].color[1] = 0.0f;
	vertex[3].color[2] = 0.0f;
	vertex[3].color[3] = 1.0f;

	vertex[4].color[0] = 1.0f;
	vertex[4].color[1] = 0.0f;
	vertex[4].color[2] = 0.0f;
	vertex[4].color[3] = 1.0f;

	vertex[5].color[0] = 1.0f;
	vertex[5].color[1] = 0.0f;
	vertex[5].color[2] = 0.0f;
	vertex[5].color[3] = 0.0f;

	vertex[6].color[0] = 1.0f;
	vertex[6].color[1] = 0.0f;
	vertex[6].color[2] = 0.0f;
	vertex[6].color[3] = 1.0f;

	vertex[7].color[0] = 1.0f;
	vertex[7].color[1] = 0.0f;
	vertex[7].color[2] = 0.0f;
	vertex[7].color[3] = 0.0f;

	vertex[8].color[0] = 1.0f;
	vertex[8].color[1] = 0.0f;
	vertex[8].color[2] = 0.0f;
	vertex[8].color[3] = 0.0f;

	vertex[9].color[0] = 1.0f;
	vertex[9].color[1] = 0.0f;
	vertex[9].color[2] = 0.0f;
	vertex[9].color[3] = 0.0f;
	vertex[10].color[0] = 1.0f;
	vertex[10].color[1] = 0.0f;
	vertex[10].color[2] = 0.0f;
	vertex[10].color[3] = 0.0f;
	vertex[11].color[0] = 1.0f;
	vertex[11].color[1] = 0.0f;
	vertex[11].color[2] = 0.0f;
	vertex[11].color[3] = 0.0f;

	vertex[0].texel[0] = 0.25f;
	vertex[0].texel[1] = 0.50f;
	vertex[1].texel[0] = 0.5f;
	vertex[1].texel[1] = 0.5f;
	vertex[2].texel[0] = 0.5f;
	vertex[2].texel[1] = 0.25f;
	vertex[3].texel[0] = 0.25f;
	vertex[3].texel[1] = 0.25f;

	vertex[4].texel[0] = 0.75f;
	vertex[4].texel[1] = 0.5f;
	vertex[5].texel[0] = 1.0f;
	vertex[5].texel[1] = 0.5f;
	vertex[6].texel[0] = 1.0f;
	vertex[6].texel[1] = 0.25f;
	vertex[7].texel[0] = 0.75f;
	vertex[7].texel[1] = 0.25f;

	vertex[8].texel[0] = 0.25f;
	vertex[8].texel[1] = 0.5f;
	vertex[9].texel[0] = 0.5f;
	vertex[9].texel[1] = 0.5f;
	vertex[10].texel[0] = 0.5f;
	vertex[10].texel[1] = 0.25f;
	vertex[10].texel[0] = 0.25f;
	vertex[10].texel[1] = 0.25f;

	vertex[11].texel[0] = 0.0f;
	vertex[11].texel[1] = 0.5f;
	vertex[12].texel[0] = 0.25f;
	vertex[12].texel[1] = 0.5f;
	vertex[13].texel[0] = 0.25f;
	vertex[13].texel[1] = 0.25f;
	vertex[14].texel[0] = 0.0f;
	vertex[14].texel[1] = 0.25f;


	/*Index of Poly / Triangle to Draw */

	triangles[0] = 0;   triangles[1] = 1;   triangles[2] = 2; // Front
	triangles[3] = 2;   triangles[4] = 3;   triangles[5] = 0;

	triangles[6] = 2;   triangles[7] = 6;   triangles[8] = 5; // Right
	triangles[9] = 5;   triangles[10] = 3;   triangles[11] = 2;

	triangles[12] = 0;   triangles[13] = 3;   triangles[14] = 5; // Top
	triangles[15] = 5;   triangles[16] = 4;   triangles[17] = 0;

	triangles[18] = 7;   triangles[19] = 1;   triangles[20] = 0; // Left
	triangles[21] = 0;   triangles[22] = 4;   triangles[23] = 7;

	triangles[24] = 6;   triangles[25] = 2;   triangles[26] = 1; // Bottom
	triangles[27] = 1;   triangles[28] = 7;   triangles[29] = 6;

	triangles[30] = 4;   triangles[31] = 5;   triangles[32] = 6; // Back
	triangles[33] = 6;   triangles[34] = 7;   triangles[35] = 4;


	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 36, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Vertex Shader which would normally be loaded from an external file */
	const char* vs_src = "#version 400\n\r"
		"in vec4 sv_position;"
		"in vec4 sv_color;"
		"in vec2 sv_texel;"
		"out vec4 color;"
		"out vec2 texel;"
		"void main() {"
		"	color = sv_color;"
		"	texel = sv_texel;"
		"	gl_Position = sv_position;"
		"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER); //Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL); // Set the shaders source
	glCompileShader(vsid); //Check that the shader compiles

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	/* Fragment Shader which would normally be loaded from an external file */
	const char* fs_src = "#version 400\n\r"
		"uniform sampler2D f_texture;"
		"in vec4 color;"
		"in vec2 texel;"
		"out vec4 fColor;"
		"void main() {"
		//"	fColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);"
		"	fColor = texture(f_texture, texel.st);"
		"}"; //Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);
	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	//Create program in GPU
	glAttachShader(progID, vsid); //Attach Vertex Shader to Program
	glAttachShader(progID, fsid); //Attach Fragment Shader to Program
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to);
	glBindTexture(GL_TEXTURE_2D, to);

	//Wrap around
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Filtering
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Bind to OpenGL
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(GL_TEXTURE_2D, //2D Texture Image
		0, //Mipmapping Level 
		GL_RGBA, //GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width, //Width
		height, //Height
		0, //Border
		GL_RGBA, //Bitmap
		GL_UNSIGNED_BYTE, img_data);

	// Find variables in the shader
	//https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
	texelID = glGetAttribLocation(progID, "sv_texel");
	textureID = glGetUniformLocation(progID, "f_texture");
}

void Game::update()
{
	elapsed = clock.getElapsedTime();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		for (int i = 0; i < 8; i++)
		{
			cube[i] = Matrix3::RotationY(-.1) * cube[i];

	}
		vertex[0].coordinate[0] = (cube.at(0).x); //3
		vertex[0].coordinate[1] = (cube.at(0).y); //3
		vertex[0].coordinate[2] = (cube.at(0).z); //3

		vertex[1].coordinate[0] = (cube.at(1).x); //1
		vertex[1].coordinate[1] = (cube.at(1).y); //1
		vertex[1].coordinate[2] = (cube.at(1).z); //1

		vertex[2].coordinate[0] = (cube.at(2).x); //2
		vertex[2].coordinate[1] = (cube.at(2).y); //2
		vertex[2].coordinate[2] = (cube.at(2).z); //2

		vertex[3].coordinate[0] = (cube.at(3).x); //3
		vertex[3].coordinate[1] = (cube.at(3).y); //3
		vertex[3].coordinate[2] = (cube.at(3).z); //3

		vertex[4].coordinate[0] = (cube.at(4).x); //1
		vertex[4].coordinate[1] = (cube.at(4).y); //1
		vertex[4].coordinate[2] = (cube.at(4).z); //1

		vertex[5].coordinate[0] = (cube.at(5).x); //2
		vertex[5].coordinate[1] = (cube.at(5).y); //2
		vertex[5].coordinate[2] = (cube.at(5).z); //2

		vertex[6].coordinate[0] = (cube.at(6).x); //1
		vertex[6].coordinate[1] = (cube.at(6).y); //1
		vertex[6].coordinate[2] = (cube.at(6).z); //1

		vertex[7].coordinate[0] = (cube.at(7).x); //2
		vertex[7].coordinate[1] = (cube.at(7).y); //2
		vertex[7].coordinate[2] = (cube.at(7).z); //2
}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		for (int i = 0; i < 8; i++)
		{
			cube[i] = Matrix3::RotationY(.1) * cube[i];

		}

		vertex[0].coordinate[0] = (cube.at(0).x); //3
		vertex[0].coordinate[1] = (cube.at(0).y); //3
		vertex[0].coordinate[2] = (cube.at(0).z); //3

		vertex[1].coordinate[0] = (cube.at(1).x); //1
		vertex[1].coordinate[1] = (cube.at(1).y); //1
		vertex[1].coordinate[2] = (cube.at(1).z); //1

		vertex[2].coordinate[0] = (cube.at(2).x); //2
		vertex[2].coordinate[1] = (cube.at(2).y); //2
		vertex[2].coordinate[2] = (cube.at(2).z); //2

		vertex[3].coordinate[0] = (cube.at(3).x); //3
		vertex[3].coordinate[1] = (cube.at(3).y); //3
		vertex[3].coordinate[2] = (cube.at(3).z); //3

		vertex[4].coordinate[0] = (cube.at(4).x); //1
		vertex[4].coordinate[1] = (cube.at(4).y); //1
		vertex[4].coordinate[2] = (cube.at(4).z); //1

		vertex[5].coordinate[0] = (cube.at(5).x); //2
		vertex[5].coordinate[1] = (cube.at(5).y); //2
		vertex[5].coordinate[2] = (cube.at(5).z); //2

		vertex[6].coordinate[0] = (cube.at(6).x); //1
		vertex[6].coordinate[1] = (cube.at(6).y); //1
		vertex[6].coordinate[2] = (cube.at(6).z); //1

		vertex[7].coordinate[0] = (cube.at(7).x); //2
		vertex[7].coordinate[1] = (cube.at(7).y); //2
		vertex[7].coordinate[2] = (cube.at(7).z); //2
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		for (int i = 0; i < 8; i++)
		{
			cube[i] = Matrix3::RotationX(-.1) * cube[i];

		}

		vertex[0].coordinate[0] = (cube.at(0).x); //3
		vertex[0].coordinate[1] = (cube.at(0).y); //3
		vertex[0].coordinate[2] = (cube.at(0).z); //3

		vertex[1].coordinate[0] = (cube.at(1).x); //1
		vertex[1].coordinate[1] = (cube.at(1).y); //1
		vertex[1].coordinate[2] = (cube.at(1).z); //1

		vertex[2].coordinate[0] = (cube.at(2).x); //2
		vertex[2].coordinate[1] = (cube.at(2).y); //2
		vertex[2].coordinate[2] = (cube.at(2).z); //2

		vertex[3].coordinate[0] = (cube.at(3).x); //3
		vertex[3].coordinate[1] = (cube.at(3).y); //3
		vertex[3].coordinate[2] = (cube.at(3).z); //3

		vertex[4].coordinate[0] = (cube.at(4).x); //1
		vertex[4].coordinate[1] = (cube.at(4).y); //1
		vertex[4].coordinate[2] = (cube.at(4).z); //1

		vertex[5].coordinate[0] = (cube.at(5).x); //2
		vertex[5].coordinate[1] = (cube.at(5).y); //2
		vertex[5].coordinate[2] = (cube.at(5).z); //2

		vertex[6].coordinate[0] = (cube.at(6).x); //1
		vertex[6].coordinate[1] = (cube.at(6).y); //1
		vertex[6].coordinate[2] = (cube.at(6).z); //1

		vertex[7].coordinate[0] = (cube.at(7).x); //2
		vertex[7].coordinate[1] = (cube.at(7).y); //2
		vertex[7].coordinate[2] = (cube.at(7).z); //2

	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		for (int i = 0; i < 8; i++)
		{
			cube[i] = Matrix3::RotationX(.1) * cube[i];

		}
		vertex[0].coordinate[0] = (cube.at(0).x); //3
		vertex[0].coordinate[1] = (cube.at(0).y); //3
		vertex[0].coordinate[2] = (cube.at(0).z); //3

		vertex[1].coordinate[0] = (cube.at(1).x); //1
		vertex[1].coordinate[1] = (cube.at(1).y); //1
		vertex[1].coordinate[2] = (cube.at(1).z); //1

		vertex[2].coordinate[0] = (cube.at(2).x); //2
		vertex[2].coordinate[1] = (cube.at(2).y); //2
		vertex[2].coordinate[2] = (cube.at(2).z); //2

		vertex[3].coordinate[0] = (cube.at(3).x); //3
		vertex[3].coordinate[1] = (cube.at(3).y); //3
		vertex[3].coordinate[2] = (cube.at(3).z); //3

		vertex[4].coordinate[0] = (cube.at(4).x); //1
		vertex[4].coordinate[1] = (cube.at(4).y); //1
		vertex[4].coordinate[2] = (cube.at(4).z); //1

		vertex[5].coordinate[0] = (cube.at(5).x); //2
		vertex[5].coordinate[1] = (cube.at(5).y); //2
		vertex[5].coordinate[2] = (cube.at(5).z); //2

		vertex[6].coordinate[0] = (cube.at(6).x); //1
		vertex[6].coordinate[1] = (cube.at(6).y); //1
		vertex[6].coordinate[2] = (cube.at(6).z); //1

		vertex[7].coordinate[0] = (cube.at(7).x); //2
		vertex[7].coordinate[1] = (cube.at(7).y); //2
		vertex[7].coordinate[2] = (cube.at(7).z); //2
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		std::cout << "Translate attempt forward" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			float z = cube[i].z;
			cube[i].z = 1.0f;

			cube[i] = Matrix3::Translate(-0.01, 0) * cube[i];

			vertex[i].coordinate[0] = (cube.at(i).x); //3
			vertex[i].coordinate[1] = (cube.at(i).y); //3

			cube[i].z = z;
		}

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		std::cout << "Translate attempt back" << std::endl;


		for (int i = 0; i < 8; i++)
		{
			float z = cube[i].z;
			cube[i].z = 1.0f;

			cube[i] = Matrix3::Translate(0.01, 0) * cube[i];

			vertex[i].coordinate[0] = (cube.at(i).x); //3
			vertex[i].coordinate[1] = (cube.at(i).y); //3

			cube[i].z = z;
		}

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		std::cout << "Scale attempt smol" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			cube[i] = Matrix3().Scale3D(99) * cube[i];

		}
		vertex[0].coordinate[0] = (cube.at(0).x); //3
		vertex[0].coordinate[1] = (cube.at(0).y); //3
		vertex[0].coordinate[2] = (cube.at(0).z); //3

		vertex[1].coordinate[0] = (cube.at(1).x); //1
		vertex[1].coordinate[1] = (cube.at(1).y); //1
		vertex[1].coordinate[2] = (cube.at(1).z); //1

		vertex[2].coordinate[0] = (cube.at(2).x); //2
		vertex[2].coordinate[1] = (cube.at(2).y); //2
		vertex[2].coordinate[2] = (cube.at(2).z); //2

		vertex[3].coordinate[0] = (cube.at(3).x); //3
		vertex[3].coordinate[1] = (cube.at(3).y); //3
		vertex[3].coordinate[2] = (cube.at(3).z); //3

		vertex[4].coordinate[0] = (cube.at(4).x); //1
		vertex[4].coordinate[1] = (cube.at(4).y); //1
		vertex[4].coordinate[2] = (cube.at(4).z); //1

		vertex[5].coordinate[0] = (cube.at(5).x); //2
		vertex[5].coordinate[1] = (cube.at(5).y); //2
		vertex[5].coordinate[2] = (cube.at(5).z); //2

		vertex[6].coordinate[0] = (cube.at(6).x); //1
		vertex[6].coordinate[1] = (cube.at(6).y); //1
		vertex[6].coordinate[2] = (cube.at(6).z); //1

		vertex[7].coordinate[0] = (cube.at(7).x); //2
		vertex[7].coordinate[1] = (cube.at(7).y); //2
		vertex[7].coordinate[2] = (cube.at(7).z); //2
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		std::cout << "Scale attempt big" << std::endl;

		for (int i = 0; i < 8; i++)
		{
			cube[i] = Matrix3().Scale3D(101) * cube[i];

		}
		vertex[0].coordinate[0] = (cube.at(0).x); //3
		vertex[0].coordinate[1] = (cube.at(0).y); //3
		vertex[0].coordinate[2] = (cube.at(0).z); //3

		vertex[1].coordinate[0] = (cube.at(1).x); //1
		vertex[1].coordinate[1] = (cube.at(1).y); //1
		vertex[1].coordinate[2] = (cube.at(1).z); //1

		vertex[2].coordinate[0] = (cube.at(2).x); //2
		vertex[2].coordinate[1] = (cube.at(2).y); //2
		vertex[2].coordinate[2] = (cube.at(2).z); //2

		vertex[3].coordinate[0] = (cube.at(3).x); //3
		vertex[3].coordinate[1] = (cube.at(3).y); //3
		vertex[3].coordinate[2] = (cube.at(3).z); //3

		vertex[4].coordinate[0] = (cube.at(4).x); //1
		vertex[4].coordinate[1] = (cube.at(4).y); //1
		vertex[4].coordinate[2] = (cube.at(4).z); //1

		vertex[5].coordinate[0] = (cube.at(5).x); //2
		vertex[5].coordinate[1] = (cube.at(5).y); //2
		vertex[5].coordinate[2] = (cube.at(5).z); //2

		vertex[6].coordinate[0] = (cube.at(6).x); //1
		vertex[6].coordinate[1] = (cube.at(6).y); //1
		vertex[6].coordinate[2] = (cube.at(6).z); //1

		vertex[7].coordinate[0] = (cube.at(7).x); //2
		vertex[7].coordinate[1] = (cube.at(7).y); //2
		vertex[7].coordinate[2] = (cube.at(7).z); //2
	}

#if (DEBUG >= 2)
	DEBUG_MSG("Update up...");
#endif

}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Drawing...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 36, vertex, GL_STATIC_DRAW);

	/*	Draw Triangle from VBO	(set where to start from as VBO can contain
		model components that 'are' and 'are not' to be drawn )	*/

	//Set Active Texture .... 32
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0);

	// Set pointers for each parameter
	// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(texelID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(texelID);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	window.display();

}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);
	glDeleteBuffers(1, vbo);
	stbi_image_free(img_data); //Free image
}

