#include <glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>
#include <stdexcept>

#include <cstdio>

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

#include "../support/error.hpp"
#include "../support/context.hpp"

namespace
{
	constexpr int kInitialWindowWidth = 1280;
	constexpr int kInitialWindowHeight = 720;
	constexpr char const* kWindowTitle = "COMP3811-Exercise 1";

	void glfw_callback_error_( int, char const* );

	void glfw_callback_key_( GLFWwindow*, int, int, int, int );


	struct GLFWCleanupHelper
	{
		~GLFWCleanupHelper();
	};
}

int main() try
{
	if( GLFW_TRUE != glfwInit() )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwInit() failed with '%s' (%d)", msg, ecode );
	}

	// Ensure that we call glfwTerminate() at the end of the program.
	GLFWCleanupHelper cleanupHelper;

	glfwSetErrorCallback( &glfw_callback_error_ );

	glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
	glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE ); // Prevent resizing for now...

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

#	if !defined(NDEBUG)
	// When building in debug mode, request an OpenGL debug context. This
	// enables additional debugging features. However, this can carry extra
	// overheads. We therefore do not do this for release builds.
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#	endif // ~ !NDEBUG

	GLFWwindow* window = glfwCreateWindow(
		kInitialWindowWidth,
		kInitialWindowHeight,
		kWindowTitle,
		nullptr, nullptr
	);

	if( !window )
	{
		char const* msg = nullptr;
		int ecode = glfwGetError( &msg );
		throw Error( "glfwCreateWindow() failed with '%s' (%d)", msg, ecode );
	}

	int iwidth, iheight;
	glfwGetFramebufferSize( window, &iwidth, &iheight );

	assert( iwidth >= 0 && iheight >= 0 );
	auto const fbwidth = std::size_t(iwidth);
	auto const fbheight = std::size_t(iheight);

	glfwSetKeyCallback( window, &glfw_callback_key_ );

	glfwMakeContextCurrent( window );
	glfwSwapInterval( 1 );

	Context context( fbwidth, fbheight );
	Surface surface( fbwidth, fbheight );

	while( !glfwWindowShouldClose( window ) )
	{
		// Let GLFW process events
		glfwWaitEvents();
		//glfwPollEvents();

		// Update state
		// Nothing to do at the moment...
	
		// Draw scene
		surface.clear();

		//TODO: drawing code goes here

		context.draw( surface );

		// Display results
		glfwSwapBuffers( window );
	}

	// Cleanup.
	// For now, all objects are automatically cleaned up when they go out of
	// scope.
	
	return 0;
}
catch( std::exception const& eErr )
{
	std::fprintf( stderr, "Top-level Exception (%s):\n", typeid(eErr).name() );
	std::fprintf( stderr, "%s\n", eErr.what() );
	std::fprintf( stderr, "Bye.\n" );
	return 1;
}


namespace
{
	void glfw_callback_error_( int aErrNum, char const* aErrDesc )
	{
		std::fprintf( stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum );
	}

	void glfw_callback_key_( GLFWwindow* aWindow, int aKey, int, int aAction, int )
	{
		if( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction )
		{
			glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
			return;
		}
	}

}

namespace
{
	GLFWCleanupHelper::~GLFWCleanupHelper()
	{
		glfwTerminate();
	}
}


