#!/usr/bin/env python3
import pathlib, sys
HERE=pathlib.Path(__file__).resolve().parent
sys.path.insert(0,str(HERE.parents[2]/'_infra'))
from corretor import main
if len(sys.argv)>2:
    print('Uso: python3 corrigir.py [SOLUCAO.c|EXECUTAVEL]');sys.exit(2)
solucao=sys.argv[1] if len(sys.argv)==2 else HERE.parents[1]/'respostas'/'t09.c'
sys.exit(main(HERE,solucao))
