#include "socket.hh"

#include <cstdlib>
#include <iostream>
#include <span>
#include <string>

using namespace std;

void get_URL( const string& host, const string& path ) {
  Address my_address(host, "http");
  TCPSocket my_socket;
  my_socket.connect(my_address);

  string str_command = "GET " + path + " HTTP/1.1\r\n";
  string str_host = "Host: " + host + "\r\n";
  string str_connection = "Connection: close\r\n\r\n";
  my_socket.write(str_command);
  my_socket.write(str_host);
  my_socket.write(str_connection);
  
  while (!my_socket.eof()) {
    string my_buffer = "";
    my_socket.read(my_buffer);
    cout << my_buffer;
  }
  
  my_socket.close();
}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    std::span<char*> args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
