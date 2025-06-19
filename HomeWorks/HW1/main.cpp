#include <Eigen/Dense>
#include <iostream>

#include <drake/multibody/plant/multibody_plant.h>
#include <drake/multibody/parsing/parser.h>

int main(int argc, char* argv[])
{    

    Eigen::Array<double,4,1> a;
    Eigen::Array<double,4,1> b;
    Eigen::Array<double,3,1> c;

    b << 1, 2, 3, 4;
    c << 5, 6, 7;

    a << 1, c;

    std::string urdf_name = "../iiwa.urdf";

    drake::multibody::MultibodyPlant<double> plant(0.0);

    drake::multibody::Parser parser(&plant);
    parser.AddModels(urdf_name);  // Укажите путь к модели робота
    plant.Finalize();

    std::unique_ptr<drake::systems::Context<double>> context = plant.CreateDefaultContext();

    Eigen::Array<double,7,1> thetta;
    thetta << 0,0,0,0,0,0,0;

    plant.SetPositions(context.get(), thetta);

    auto X_WE = plant.CalcRelativeTransform(*context, plant.GetFrameByName("iiwa_link_0"), plant.GetFrameByName("iiwa_link_ee"));

    Eigen::Vector3d position = X_WE.translation().transpose();
    Eigen::Matrix3d rotation = X_WE.rotation().matrix();

    std::cout << "A? " << a.transpose() << std::endl;

    std::cout << "Position: " << position.transpose() << std::endl;
}