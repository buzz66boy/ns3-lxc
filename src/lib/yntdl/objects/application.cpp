#include <string>
#include <iostream>
#include "application.h"

using namespace yntdl;

Application::Application(const Application& temp): Nameable(temp), AdditionalTags(temp) {
    path = temp.path;
    commands = temp.commands;
    inherit = temp.inherit;
}

Application::Application(Application *temp): Nameable(*temp), AdditionalTags(*temp) {
    path = temp->path;
    // Enforce inheritance
    for(auto cmdPair : temp->commands){
        if(cmdPair.second){
            commands.push_back(cmdPair);
        }
        // else {
        //     std::cout << "Filtering out cmd " + cmdPair.first + " for app " + name << std::endl; 
        // }
    }
    inherit = temp->inherit;
}