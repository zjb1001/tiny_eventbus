#include "gtest/gtest.h"
#include "../src/eventbus.h"
#include <string.h>
#include <regex>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// common utility function to check if the received message matches the expected message with timestamp
bool check_message_with_regex(const char* received_message, const char* expected_message) {
    std::regex message_regex(std::string("^") + std::string(expected_message) + R"( \[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\]\s*$)");
    return std::regex_match(received_message, message_regex);
}

// Test fixture for EventBus
class EventBusTest : public ::testing::Test {
protected:
    EventBus* bus; // Declare the bus variable here

    void SetUp() override {
        // Initialize the event bus before each test
        ASSERT_TRUE(eventbus_init("test_bus"));
        bus = eventbus_connect("test_bus") ? eventbus_get_bus() : nullptr;
        ASSERT_NE(bus, nullptr);
    }

    void TearDown() override {
        // Clean up the event bus after each test
        eventbus_cleanup();
    }
};

// Test case for initializing the event bus
TEST_F(EventBusTest, InitializeEventBus) {
    // The event bus should be initialized in the fixture's SetUp
    // Just check that the bus name is set correctly
    EXPECT_STREQ("test_bus", bus->name);
}

// Test case for creating a topic
TEST_F(EventBusTest, CreateTopic) {
    int topic_id = eventbus_create_topic("test_topic");
    EXPECT_GE(topic_id, 0);
}

// Test case for subscribing to a topic
TEST_F(EventBusTest, SubscribeToTopic) {
    int topic_id = eventbus_create_topic("test_topic");
    ASSERT_GE(topic_id, 0);

    int result = eventbus_subscribe_to_topic("test_topic");
    EXPECT_EQ(result, 0);
}

// Test case for publishing a message
TEST_F(EventBusTest, PublishMessage) {
    int topic_id = eventbus_create_topic("test_topic");
    ASSERT_GE(topic_id, 0);

    const char* message = "Hello, World!";
    int result = eventbus_publish_message("test_topic", message);
    EXPECT_EQ(result, 0);
}

// New test case for multiple publishers and multiple subscribers across multiple processes
TEST_F(EventBusTest, MultiplePublishersAndSubscribersAcrossProcesses) {
    // Create two topics
    int topic_id1 = eventbus_create_topic("topic1");
    int topic_id2 = eventbus_create_topic("topic2");
    ASSERT_GE(topic_id1, 0);
    ASSERT_GE(topic_id2, 0);

    // init message for topic1 and topic2
    const char *message1 = "Message for topic1";
    const char *message2 = "Message for topic2";
    
    // Fork to create child processes for publishers and subscribers
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child process 1 (Publisher for topic1)
        eventbus_publish_message("topic1", message1);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child process 2 (Publisher for topic2)
        eventbus_publish_message("topic2", message2);
        exit(0);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        // Child process 3 (Subscriber for topic1)
        eventbus_subscribe_to_topic("topic1");
        char* received_message = eventbus_read_message("topic1");
        // ASSERT_STREQ(message, "Message for topic1");
        EXPECT_TRUE(check_message_with_regex(received_message, message1));
        exit(0);
    }

    pid_t pid4 = fork();
    if (pid4 == 0) {
        // Child process 4 (Subscriber for topic2)
        eventbus_subscribe_to_topic("topic2");
        char* received_message = eventbus_read_message("topic2");
        // ASSERT_STREQ(message, "Message for topic2");
        EXPECT_TRUE(check_message_with_regex(received_message, message2));
        exit(0);
    }

    // Wait for all child processes to finish
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
    waitpid(pid3, nullptr, 0);
    waitpid(pid4, nullptr, 0);
}

// Test case for reading a message with subscription (positive response)
TEST_F(EventBusTest, ReadMessagePositive) {
    int topic_id = eventbus_create_topic("test_topic");
    ASSERT_GE(topic_id, 0);

    int subscribe_result = eventbus_subscribe_to_topic("test_topic");
    ASSERT_EQ(subscribe_result, 0);

    const char* message = "Hello, World!";
    int publish_result = eventbus_publish_message("test_topic", message);
    ASSERT_EQ(publish_result, 0);

    char* received_message = eventbus_read_message("test_topic");
    // Use a regular expression to match the message with timestamp
    EXPECT_TRUE(check_message_with_regex(received_message, message));
}

// Test case for reading a message without subscription (negative response)
TEST_F(EventBusTest, ReadMessageNegative) {
    int topic_id = eventbus_create_topic("test_topic");
    ASSERT_GE(topic_id, 0);

    const char* message = "Hello, World!";
    int publish_result = eventbus_publish_message("test_topic", message);
    ASSERT_EQ(publish_result, 0);

    char* received_message = eventbus_read_message("test_topic");
    EXPECT_EQ(received_message, nullptr);
}

// New test case for multiple topics and multiple subscribers across multiple processes
TEST_F(EventBusTest, MultipleTopicsAndSubscribersAcrossProcesses) {
    // Create two topics
    int topic_id1 = eventbus_create_topic("topic1");
    int topic_id2 = eventbus_create_topic("topic2");
    ASSERT_GE(topic_id1, 0);
    ASSERT_GE(topic_id2, 0);

    // Init message
    const char* message1 = "Message for topic1";
    const char* message2 = "Message for topic2";

    // Fork to create child processes for publishers and subscribers
    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child process 1 (Publisher for topic1)
        eventbus_publish_message("topic1", message1);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child process 2 (Publisher for topic2)
        eventbus_publish_message("topic2", message2);
        exit(0);
    }

    pid_t pid3 = fork();
    if (pid3 == 0) {
        // Child process 3 (Subscriber for topic1)
        eventbus_subscribe_to_topic("topic1");
        char* received_message = eventbus_read_message("topic1");
        // ASSERT_STREQ(message, message1);
        EXPECT_TRUE(check_message_with_regex(received_message, message1));
        exit(0);
    }

    pid_t pid4 = fork();
    if (pid4 == 0) {
        // Child process 4 (Subscriber for topic2)
        eventbus_subscribe_to_topic("topic2");
        char* received_message = eventbus_read_message("topic2");
        // ASSERT_STREQ(message, "Message for topic2");
        EXPECT_TRUE(check_message_with_regex(received_message, message2));
        exit(0);
    }

    // Wait for all child processes to finish
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
    waitpid(pid3, nullptr, 0);
    waitpid(pid4, nullptr, 0);
}
