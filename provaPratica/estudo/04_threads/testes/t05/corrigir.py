#!/usr/bin/env python3
import pathlib, sys
sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[3] / '_infra'))
from corretor import main
if len(sys.argv) != 2:
    print('Uso: python3 corrigir.py SOLUCAO.c|EXECUTAVEL|RESPOSTAS.json')
    sys.exit(2)
sys.exit(main(pathlib.Path(__file__).parent, sys.argv[1]))
