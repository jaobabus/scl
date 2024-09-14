

# SCL - Simple Command Lexer

**SCL** is a lightweight library for implementing a command-line interface in microcontrollers and embedded systems. It eliminates the need for routine command-line handling, allowing developers to focus solely on writing command handlers.

#### Key Features:
1. **Argument Handling**: All arguments are first parsed into user-defined objects before being passed to the handler.

2. **Character Buffer**: The `SCLConsoleBufferContext` takes care of all terminal input handling.

3. **Minimal Memory**: Dynamic memory is only required for storing user-defined argument objects.

4. **Component Independence**: Each component of the SCL library can be used independently.

#### Example:

```c++

SCLC_DeclareCommand(echo, StringView sv) {
	fwrite(sv.data(), sv.size(), 1, stdout);
	fwrite("\n", 1, 1, stdout);
	return SCLE_NoError;
}

SCLC_DeclareCommand(add, int a, int b) {
	printf("%d + %d = %d\n", a, b, a + b);
	return SCLE_NoError;
}

/* SCLC_DeclareCommand will expanded to: 
void command_add_dummy_args(int a, int b);
struct command_add_type
    : ArgsCommandWrapper<command_add_type, decltype(&command_add_dummy_args)> {
  struct Body {
    uint8_t execute();
    int a;
    int b;
  };
  static inline SCLExecuteError sc_execute(void* obj_buffer,
                                           const uint16_t* flags, size_t size) {
    return {reinterpret_cast<Body*>(obj_buffer)->execute(), 0};
  }
  static constexpr SCLCommandDescriptorWithName<sizeof("add")> sc_descriptor{
      sc_descriptor_base, {"add"}};
} command_add;
uint8_t command_add_type::Body::execute() {
  printf("%d + %d = %d\n", a, b, a + b);
  return SCLE_NoError;
}
*/


constexpr const SCLCommandDescriptor* commands[] = {
	&command_echo.sc_descriptor.base,
	&command_add.sc_descriptor.base,
};


int main() {
	termios tios;
	tcgetattr(fileno(stdin), &tios);
	tios.c_lflag &= ~ECHO & ~ICANON;
	tcsetattr(fileno(stdin), 0, &tios);

	auto write = +[](void*, const void* data, size_t size) {
		fwrite(data, size, 1, stdout);
	};
	auto execute = +[](void*, void* cmd, size_t size) {
		auto alloc = SCLAllocator{malloc, +[](void* d, size_t) { free(d); }};
		auto err = scl_find_and_execute_inplace(commands, sizeof(commands) / sizeof(commands[0]),
		                                        &alloc, (char*)cmd, size);
		if (err.error)
			fprintf(stderr, "Error %d at %d\n", err.error, err.token);
	};

	char cmd_buffer[128];
	auto colors = (const uint8_t*)"\x0F\x0F\xC4\x0F\x94\x40\xC6\xD0\xFD\x0F\x0F\x0F\x0F\xC4\xC4\xC4";
	SCLConsoleBufferContext buffer = sclcb_make_context(
	                                     cmd_buffer, sizeof(cmd_buffer),
	                                     nullptr, write, execute,
	                                     colors );
	while (true) {
		int ch = getchar();
		if (ch != -1)
			sclcb_on_char(&buffer, (uint8_t)ch);
	}
}
```

