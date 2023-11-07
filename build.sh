#!/bin/bash

# Função para configurar e construir o projeto
setup_and_build() {
  build_dir=$1
  source_dir=$2
  cmake_options=$3

  # Verifica se o diretório de build existe
  if [ -d "$build_dir" ]; then
    # Remove o conteúdo do diretório de build existente
    rm -rf "$build_dir"/*
  else
    # Cria o diretório de build
    mkdir -p "$build_dir"
  fi

  # Entra no diretório de build e executa o cmake e o make
  pushd "$build_dir"
  if [ -n "$cmake_options" ]; then
    cmake $cmake_options "$source_dir" && make -j8
  else
    cmake "$source_dir" && make -j8
  fi
  popd
}

# Configuração para ARM
cmake_options_arm="-DCROSS_COMPILE_ARM=on"
setup_and_build "build/build_arm" "../../src/Eys" "$cmake_options_arm"

# Configuração para PC Linux 64-bit
cmake_options_pc=""
setup_and_build "build/build_pc_linux_64" "../../src/Eys" "$cmake_options_pc"

echo "Build has been completed."
