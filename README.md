# 3D Cellular Automata

Этот проект представляет собой реализацию игры "__Жизнь__" в трехмерном пространстве с использованием языка программирования C++. Для графики используется [OpenGl](https://ru.wikipedia.org/wiki/OpenGL) вместе с [glm](https://github.com/g-truc/glm), а также [glad](https://github.com/Dav1dde/glad), [GLFW](https://github.com/glfw/glfw), [range-v3](https://github.com/ericniebler/range-v3), [ImGui](https://github.com/ocornut/imgui) и [spdlog](https://github.com/gabime/spdlog).

## Описание

Игра "__Жизнь__" – это *клеточный автомат*, в котором каждая клетка находится в одном из трех состояний: живая, умирающая или мертвая. В каждом шаге клетки изменяют свое состояние в соответствии с определенными правилами (задаваемыми пользователем), основанными на состоянии соседних клеток.

Посмотреть больше про правила можно здесь <ссылка>

В данной реализации мы расширяем классическую игру "__Жизнь__" на трехмерное пространство, что добавляет интересные возможности.

## Сборка

Чтобы использовать эту программу, вам потребуется компилятор C++ (`clang` или `MSVC`), поддерживающий `C++20`, а также утилита `cmake` (версия не менее 3.4). Просто склонируйте репозиторий и скомпилируйте исходный код.

### Windows

#### Visual Studio 2022

Самый простой способ собрать и запустить проект будет в __Visual Studio 2022__. 

В самое IDE уже нужно будет сделать следующие шаги:
1) собрать и запустить `generate_shaders.exe` всего 1 раз
2) собрать и запустить `cellular_automata.exe`

#### Ручная сборка 

WIP

### Linux

Чтобы собрать, делаем следующие шаги:
1) `git clone https://github.com/Schlypar/3D-Game-of-Life.git`
2) `cd 3D-Game-of-Life`
3) `cmake -S . -B build/ -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++`
4) `cmake --build build/ --target=all`
5) `./build/cellular_automata`
