#ifndef HAVOCCLIENT_COMMON_H
#define HAVOCCLIENT_COMMON_H

#define CPPHTTPLIB_OPENSSL_SUPPORT

/* std libraries */
#include <memory>

/* Qt libraries */
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QMessageBox>
#include <QToolButton>
#include <QFileDialog>
#include <QMenu>

/* external libs */
#include <toml.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <httplib.h>

/* define namepsace */
using json = nlohmann::json;

/* typedefs */
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#endif //HAVOCCLIENT_COMMON_H
