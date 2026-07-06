#pragma once
#include "FileManager.h"

#include <iostream>
#include <fstream>
#include <memory>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class MenuContext;

class MenuState {
public:
    virtual ~MenuState() = default;
    virtual void render(MenuContext& context) = 0;
    virtual void handle(MenuContext& context) = 0;
};

class MenuContext {
    std::unique_ptr<MenuState> m_state = nullptr;
    FileManager m_fileManager;
    bool m_start = true;

public:
    explicit MenuContext(std::unique_ptr<MenuState> initialState)
        : m_state(std::move(initialState)) {
    }

    void changeState(std::unique_ptr<MenuState> newState) {
        m_state = std::move(newState);
    }
    void request() { if (m_state) m_state->handle(*this); }
    bool isOpen() const { return m_start; }
    void exit() { m_start = false; }

    FileManager& getFileManager() { return m_fileManager; }

    void run() {
        if (!m_state) return;
        m_state->render(*this);
        request();
    }
};

class MainMenu : public MenuState {
public:
    void render(MenuContext& context) override;
    void handle(MenuContext& context) override;
};

class TasksMenu : public MenuState {
public:
    void render(MenuContext& context) override;
    void handle(MenuContext& context) override;
};

// Реализация методов MainMenu
void MainMenu::render(MenuContext& context) {
    system("cls");
    std::cout << "-- Главное меню --\n";
    std::cout << "1. Открыть заметки \n";
    std::cout << "2. Выйти \n";
    std::cout << "Выберите действие: ";
}
void MainMenu::handle(MenuContext& context) {
    int choice;
    std::cin >> choice;
    switch (choice) {
    case 1:
        // Переход в меню с тасками
        context.changeState(std::make_unique<TasksMenu>());
        break;
    case 2:
        context.exit();
    }
}

// Реализация методов TasksMenu
void TasksMenu::render(MenuContext& context) {
    system("cls");
    std::cout << "-- Задачи --\n";

    const auto& tasks = context.getFileManager().getTasks();
    if (tasks.empty()) {
        std::cout << "[Список пуст]\n";
    }
    else {
        for (size_t i = 0; i < tasks.size(); ++i) {
            std::cout << i + 1 << ". [" << (tasks[i].isDone ? "x" : " ") << "] "
                << tasks[i].title << "\n";
        }
    }
    std::cout << std::endl;

    std::cout << "1. Добавить задачу \n";
    std::cout << "2. Отметить выполненной \n";
    std::cout << "3. Удалить задачу \n";
    std::cout << "4. Вернуться назад \n";
    std::cout << "Выберите действие: ";
}

void TasksMenu::handle(MenuContext& context) {
    int choice;
    std::cin >> choice;
    switch (choice) {
    case 1:
    {
        std::cout << "Введите задачу: ";
        std::string title;
        std::cin.ignore(); // очистка буфера ввода
        std::getline(std::cin, title);

        context.getFileManager().addTask(title);
        break;
    }
    case 2:
    {
        std::cout << "Введите номер задачи: ";
        int taskNumber;
        std::cin >> taskNumber;

        context.getFileManager().taskDone(taskNumber);
        break;
    }
    case 3:
    {
        std::cout << "Введите номер задачи: ";
        int taskNumber;
        std::cin >> taskNumber;

        context.getFileManager().deleteTask(taskNumber);
        break;
    }
    case 4:
        // Переход в главное меню
        context.changeState(std::make_unique<MainMenu>());
        break;
    }
}