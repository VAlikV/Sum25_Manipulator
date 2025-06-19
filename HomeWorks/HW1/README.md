## Запуск кода

Суть задания запустить код, те самым проверить правильность установки зависимостей

```bash
mkdir build && cd build
cmake ..
cmake --build .
./LibTest
```

Во время выполнения `cmake --build .` следите за оперативной памятью, чтобы ubuntu не зависла (иначе нужно будет перезагружать компьютер)

**Скопируйте то, что выводится в консоль, вставьте в текстовый файл, отправьте в moodle**

## Что происходит?

### CMakeLists.txt
В этом файле находятся инструкции по сборке проекта, при помощи которых cmake из исходных файлов билдит исполняемый (его структуру разберем позже)

### iiwa.urdf

Файл с описанием манипулятора KUKA iiwa LBR 14

### main.cpp

Исходный код программы (код не претендует на какой-то туториал и носит характер теста библиотеки):

1) Подключение необходимых библиотке

    ```cpp
    #include <iostream>

    #include <Eigen/Dense>

    #include <drake/multibody/plant/multibody_plant.h>
    #include <drake/multibody/parsing/parser.h>
    ```
2) Пример простого объявления и определения массивов в Eigen

    ```cpp
    Eigen::Array<double,4,1> a;
    Eigen::Array<double,4,1> b;
    Eigen::Array<double,3,1> c;

    b << 1, 2, 3, 4;
    c << 5, 6, 7;

    a << 1, c;
    ```

3) Создание модели манипулятора для дальнейшей работы с ним

    ```cpp
    std::string urdf_name = "../iiwa.urdf"; // Путь к модели робота

    drake::multibody::MultibodyPlant<double> plant(0.0);

    drake::multibody::Parser parser(&plant);
    parser.AddModels(urdf_name);
    plant.Finalize();

    std::unique_ptr<drake::systems::Context<double>> context = plant.CreateDefaultContext();
    ```

4) Задание вектора углов в шарнирах и решение прямой задачи кинематики

    ```cpp
    Eigen::Array<double,7,1> thetta;
    thetta << 0,0,0,0,0,0,0;

    plant.SetPositions(context.get(), thetta);

    auto X_WE = plant.CalcRelativeTransform(*context, plant.GetFrameByName("iiwa_link_0"), plant.GetFrameByName("iiwa_link_ee"));
    ```

5) Получение ветора положения и матрицы ориентации, полуенных при решении FK, вывод значений в консоль

    ```cpp
    Eigen::Vector3d position = X_WE.translation().transpose();
    Eigen::Matrix3d rotation = X_WE.rotation().matrix();

    std::cout << "A? " << a.transpose() << std::endl;
    std::cout << "Position: " << position.transpose() << std::endl;
    ```
