#!/bin/bash

# Verifica se o número de argumentos é válido
if [ $# -ne 2 ]; then
  echo "Uso: $0 <caminho/nome_do_vídeo> <caminho/diretório_de_saída>"
  exit 1
fi

# Verifica se o vídeo de entrada existe
if [ ! -f "$1" ]; then
  echo "O arquivo de vídeo $1 não existe."
  exit 1
fi

# Verifica se o diretório de saída existe, caso contrário, cria-o
if [ ! -d "$2" ]; then
  echo "O diretório de saída $2 não existe. Criando o diretório..."
  mkdir -p "$2"
fi

# Extrai os frames do vídeo usando o FFmpeg
ffmpeg -i "$1" -vf fps=4 "$2/frames_%04d.jpg"

echo "Extração de frames concluída."
