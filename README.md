# Sentinel

Automated testing software for the Sentinel Talon interface circuit board.

## Installation

Vagrant is recommended as it will provision the correct development environment automatically. Note: installation requires a Gitlab deployment password to be in the `.vault_pw` file at the top of the repo. Contact a team member to obtain it.

```sh
$ cd sentinel
$ vagrant up
$ vagrant ssh
$ # now logged into virtual build environment
$ cd /vagrant
```

To compile:

```sh
$ mkdir build
$ cd build
$ cmake ..
$ make deploy
```

## Usage

The `sentinel.toml` configuration file is expected in the directory you run this program runs from (normally `/home/admin`).

This runs in place of the RoboRIO robot program so you need to stop the auto-start feature before running.

```sh
$ /etc/init.d/nilvrt stop
$ frcKillRobot.sh
$ cd /home/admin
$ ./sentinel
```

Test results are logged in `sentinel.json`. Analysis tools are in the `data` directory of this repository.
