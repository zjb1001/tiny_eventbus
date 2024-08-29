# Tiny EventBus Service

The Tiny EventBus Service is designed to enable efficient inter-process event communication through shared memory. This service allows processes to publish and subscribe to events, facilitating real-time communication without the overhead of network calls.

## Project Structure

```
./tiny_eventbus_demo/
├── Makefile
├── src/
│   ├── eventbus.h
│   ├── eventbus.c
│   ├── init_eventbus.c
│   └── eventbus.h
├── examples/
│   ├── publisher_interative.c
│   ├── subscriber_interative.c
│   ├── publisher_command.c
│   └── subscriber_command.c
├── test/
│   └── test_eventbus.cpp
└── README.md
```
## Purpose

The primary goal of the Tiny EventBus Service is to facilitate efficient inter-process event management through a structured, engineering approach. By leveraging shared memory for IPC, this service ensures low latency and high throughput, making it ideal for high-performance applications. The project aims to provide a robust framework for publishing, subscribing to, and managing events across different processes, enabling real-time communication without the overhead of network calls.

## Features

- **Event Publishing and Subscribing:** Processes can publish events to specific topics and subscribe to topics to receive events.
- **Shared Memory IPC:** Utilizes shared memory for efficient and low-latency inter-process communication.
- **Topic Management:** Supports creation and management of topics, allowing for organized event distribution.
- **Real-time Communication:** Ensures real-time event delivery, critical for time-sensitive applications.
- **Scalable and Lightweight:** Designed to be lightweight and scalable, suitable for various application sizes and complexities.
- **Error Handling and Cleanup:** Includes mechanisms for error handling and resource cleanup to maintain system stability.

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

## Testing with Google Test (gtest)

### Installing gtest

#### Using APT Package Management (Debian/Ubuntu)

To install gtest using APT, run the following commands:

```bash
sudo apt-get update
sudo apt-get install libgtest-dev
```

Note: After installation, you may need to build gtest manually. Navigate to the gtest source directory and run:

```bash
cd /usr/src/gtest
sudo cmake .
sudo make
sudo make install
```

#### Manual Download and Installation

To manually download and install gtest, follow these steps:

1. Download the gtest source code from the [official GitHub repository](https://github.com/google/googletest).
2. Extract the downloaded archive and navigate to the extracted directory.
3. Build and install gtest:

```bash
cmake .
make
sudo make install
```

### Running Tests

To run the tests, use the following command:

```bash
make run_test
```

Contributions to the Tiny EventBus Service are welcome! Please follow the standard GitHub workflow for contributing:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Commit your changes.
4. Push your branch to your fork.
5. Submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.