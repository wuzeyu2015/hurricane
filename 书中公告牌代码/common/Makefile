BIN    := build/libcommon.a
INCLUDE_DIR := -I./include 
CXX_SRC_DIR := src
TARGET_DIR := build/targets
OBJ_DIR := build/objs

CC     	:= gcc
CXX		:= g++
LD		:= g++

CFLAGS 	:= $(INCLUDE_DIR) -Wall -O3 -fPIC
CXXFLAGS 	:=  -std=c++11

include ../builder/Makefile.static-library
