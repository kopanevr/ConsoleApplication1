#include <iostream>
#include <Windows.h>
#include <glad/glad.h>

LRESULT CALLBACK WinProc(
	HWND	hWnd,
	UINT	uMsg,
	WPARAM	wParam,
	LPARAM	lParam)																					// WindowProc (или WndProc) - это функция обработки сообщений окна (Window Procedure) в Windows API.
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);																			// PostQuitMessage — это функция, которая используется для отправки сообщения WM_QUIT в очередь сообщений текущего потока.

		return (LRESULT)0;
	case WM_SIZE:
		WORD widht = LOWORD(lParam);
		WORD hiht = HIWORD(lParam);

		glViewport((GLint)0, (GLint)0, (GLsizei)widht, (GLsizei)hiht);

		return (LRESULT)0;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

const char* vertexShaderSource =
	"#version 330 core\n"

	"layout (location = 0) in vec3 pos;\n"

	"void main()\n"
	"{\n"
	"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
	"}\n";

const char* fragmentShaderSource =
	"#version 330 core\n"

	"out vec4 FragColor;\n"

	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n";

int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPreviousInstance,

	LPSTR		lpCmdLine,																			// Этот параметр содержит параметры командой строки, переданные при запуске приложения.
	int			nCmdShow)																			// Этот параметр указывает, как изначально должно отображаться окно приложения.
{
	const char CLASS_NAME[]		= "SampleWindowClass";
	const char WINDOW_NAME[]	= "SampleWindowName";

	WNDCLASSA wc = {};
	wc.lpfnWndProc		= WinProc;
	wc.hInstance		= hInstance;
	wc.lpszClassName	= CLASS_NAME;

	if (!RegisterClassA(&wc))
	{
		std::cerr << "Faild to register window class!" << std::endl;
		return 1;
	}

	HWND hWnd = CreateWindowA(
		(LPCSTR)CLASS_NAME,
		(LPCSTR)WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,																		// Стиль окна.

		CW_USEDEFAULT,																				// Положение по X.
		CW_USEDEFAULT,																				// Положение по Y.
		800,																						// Ширина.
		600,																						// Высота.

		nullptr,																					// Дескриптор родительского окна.
		nullptr,																					// Дескриптор меню.
		hInstance,
		nullptr
	);

	if (!hWnd)
	{
		std::cerr << "Faild to create window!" << std::endl;
		return 1;
	}

	//

	HDC hDC = GetDC(hWnd);																			// GetDC — это функция, которая возвращает дескриптор контекста устройства (Device Context - DC) для указанного окна или всего экрана.

	if (!hDC) { return 1; }

	PIXELFORMATDESCRIPTOR pfd = {
		(WORD)sizeof(PIXELFORMATDESCRIPTOR),
		(WORD)1U,

		PFD_DRAW_TO_WINDOW			|
		PFD_SUPPORT_OPENGL			|
		PFD_DOUBLEBUFFER,

		PFD_TYPE_RGBA,																				// Модель RGBA (Red, Green, Blue, Alpha).

		(BYTE)32U,																					// Количество битов на пиксель.
		(BYTE)0U,
		(BYTE)0U,
		(BYTE)0U,
		(BYTE)0U,
		(BYTE)0U,
		(BYTE)0U,
		(BYTE)0U,

		(BYTE)0U,																					// Количество битов для буфера аккумуляции.
		(BYTE)0U,
		(BYTE)0U,
		(BYTE)0U,
		(BYTE)0U,

		(BYTE)24U,																					// Количество битов для буфера глубины.
		(BYTE)8U,																					// Количество битов для трафаретного буфера.

		(BYTE)0U,																					// Количество вспомогательных буферов.
		PFD_MAIN_PLANE,																				// Тип слоя.
		(BYTE)0U,

		(DWORD)0U,
		(DWORD)0U,
		(DWORD)0U
	};

	int pixelFormat = ChoosePixelFormat(hDC, (const PIXELFORMATDESCRIPTOR*)&pfd);					// ChoosePixelFormat — это функция, которая возвращает индекс наиболее подходящего формата пикселей, поддерживаемого драйвером устройства.

	if (pixelFormat == 0)
	{
		std::cerr << "Faild to shoose pixel format!" << std::endl;
		return 1;
	}

	if (!SetPixelFormat(hDC, pixelFormat, (const PIXELFORMATDESCRIPTOR*)& pfd))
	{
		std::cerr << "Faild to set pixel format!" << std::endl;
		return 1;
	}

	HGLRC hGLRC = wglCreateContext(hDC);															// wglCreateContext — это функция, которая создает Rendering Context.

	if (!hGLRC)
	{
		std::cerr << "Faild to create context!" << std::endl;
		return 1;
	}

	if (!wglMakeCurrent(hDC, hGLRC)) { return 1; }

	if (!gladLoadGL())																				// gladLoadGL — это функция, которая загружает указатели на функции OpenGL во время выполнения.
	{
		std::cerr << "Faild to initialize GLAD!" << std::endl;
		return 1;
	}

	glViewport((GLint)0, (GLint)0, (GLsizei)800, (GLsizei)600);										// glViewport — это функция, которая используется для определения размера и положения просмотра на экране.

	//
	// Создание и компиляция шейдеров

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, (GLsizei)1, (const GLchar* const*)&vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, (GLsizei)1, (const GLchar* const*)&fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Создание программы шейдеров

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);																	// Связывание скомпилированных шейдеров с программой.

	// Создание буфера вершин и массива вершин

	GLuint VAO = (GLuint)0U;
	GLuint VBO = (GLuint)0U;
	glGenVertexArrays((GLsizei)1, &VAO);															// VAO: Массив вершин.
	glGenBuffers((GLsizei)1, &VBO);																	// VBO: Буфер вершин.

	float vertices[] = {
		 0.0f,	 0.5f,	0.0f,
		 0.5f,	-0.5f,	0.0f,
		-0.5f,	-0.5f,	0.0f
	};

	glBindVertexArray(VAO);																			// Связывание VAO.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(vertices), (const void*)vertices, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0U, (GLuint)3U, GL_FLOAT, GL_FALSE, (GLsizei)(3U * sizeof(float)), (void*)0);
	glEnableVertexAttribArray((GLuint)0U);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	MSG msg = {};

	BOOL state = TRUE;

	while (state)
	{
		while (PeekMessageA((LPMSG)&msg, nullptr, (UINT)0U, (UINT)0U, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) { state = FALSE; }

			//

			glClearColor((GLfloat)0.2f, (GLfloat)0.298f, (GLfloat)0.29f, (GLfloat)1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glDrawArrays(GL_TRIANGLES, (GLint)0, (GLsizei)3);

			//

			SwapBuffers(hDC);

			TranslateMessage((const MSG*)&msg);
			DispatchMessageA((const MSG*)&msg);
		}
	}

	//

	UnregisterClassA(CLASS_NAME, hInstance);
	wglDeleteContext(hGLRC);

	return 0;
}
