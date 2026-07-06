#pragma once
#include "Struct.h"

#include <fstream>
#include <iostream>

#include <thread>
#include <chrono>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class FileManager {
private:
    const std::string FILE_NAME = "config.json";
    std::vector<Task> m_tasks;

public:
    explicit FileManager() {
        openConfigFile();
        loadData();
    }

    void openConfigFile() {
        std::ifstream file(FILE_NAME);
        if (!file.is_open()) {
            std::ofstream newFile(FILE_NAME);
            newFile << "[]";
        }
    }

    void loadData() {
        std::ifstream file(FILE_NAME);
        if (!file.is_open()) return;

        json object;
        file >> object;

        m_tasks.clear();
        for (const auto& item : object) {
            m_tasks.push_back({ item["title"], item["isDone"] });
        }
    }

    void saveData() {
        std::ofstream file(FILE_NAME);
        json object = json::array();

        for (const auto& task : m_tasks) {
            object.push_back({ { "title", task.title }, { "isDone", task.isDone } });
        }
        file << object;
    }

    const std::vector<Task>& getTasks() const { return m_tasks; }

    void addTask(const std::string& title) {
        m_tasks.push_back({ title, false });
        saveData();
    }

    void taskDone(const int taskNumber) {
        if (taskNumber < 1 || taskNumber > m_tasks.size())
        {
            std::cout << "Такой задачи нет\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return;
        }

        m_tasks[taskNumber - 1].isDone = true;
        saveData();
    }

    void deleteTask(const int taskNumber) {
        if (taskNumber < 1 || taskNumber > m_tasks.size())
        {
            std::cout << "Такой задачи нет\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return;
        }

        m_tasks.erase(m_tasks.begin() + (taskNumber - 1));
        saveData();
    }
};