#include "config.h"

Config::Config(QObject *parent) : QObject(parent)
{
    config_filename.clear();
    config_filename.append("/root/repo-github/pedestrian/pedestrian/pedestrian/config.json");
    load_config();
    save_config();
}

