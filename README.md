# Tiny EventBus Service

The Tiny EventBus Service is designed to enable efficient inter-process event communication through shared memory. This service allows processes to publish and subscribe to events, facilitating real-time communication without the overhead of network calls.

## Structure

```
/home/pg/OwnRepoground/tiny_eventbus/
├── Makefile
├── src/
│   ├── eventbus.h
│   ├── eventbus.c
├── examples
│   ├── publisher_command.c
│   ├── publisher_interative.c
│   ├── subscriber_command.c
│   └── subscriber_interative.c
```
## Purpose

The primary goal of the Tiny EventBus Service is to provide a lightweight and efficient mechanism for inter-process communication (IPC) using shared memory. This approach is particularly useful for high-performance applications where low latency and high throughput are critical.

## Setup

To set up the Tiny EventBus Service, follow these steps:

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/your-repo/tiny_eventbus_demo.git
   cd tiny_eventbus_demo
   ```

2. **Build the Service:**
   ```bash
   make
   ```

3. **Run the Service:**
   ```bash
   ./init_eventbus <bus_name>
   ```

## Usage

### Initializing the EventBus

To initialize the EventBus, run the following command:

```bash
./init_eventbus <bus_name>
```

### Connecting to the EventBus Interative Model

To connect to an existing EventBus, use the following command:

```bash
./examples/publisher_interative <bus_name>
```

or

```bash
./examples/subscriber_interative <bus_name> <subscriber_id>
```

### Publishing and Subscribing to Events by Command line Parameters

- **Publishing Events:**
  ```bash
  ./examples/publisher_command <bus_name> <topic> <data> [period]
  ```

- **Subscribing to Events:**
  ```bash
  ./examples/subscriber_command <bus_name> <topic> [subscriber_id]
  ```

## Examples

The `examples` directory contains several example programs demonstrating how to use the EventBus service:

- `publisher_interative.c`: Interactive publisher example.
- `subscriber_interative.c`: Interactive subscriber example.
- `publisher_command.c`: Command-line publisher example.
- `subscriber_command.c`: Command-line subscriber example.

## Contributing

Contributions to the Tiny EventBus Service are welcome! Please follow the standard GitHub workflow for contributing:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Commit your changes.
4. Push your branch to your fork.
5. Submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
