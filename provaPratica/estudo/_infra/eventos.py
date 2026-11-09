"""Validadores de rastros: aceitam ordens concorrentes que respeitam invariantes."""
def validar(tipo, p, texto):
    try:
        linhas=[x.split() for x in texto.splitlines() if x.strip()]
        if tipo=='grafo':
            assert all(len(x)==1 for x in linhas)
            ordem=[x[0] for x in linhas]
            assert len(ordem)==len(p['nos']) and set(ordem)==set(p['nos'])
            return all(ordem.index(a)<ordem.index(b) for a,b in p['arestas'])
        if tipo=='lote':
            dentro=set(); usados=set(); fase='entrada'; viagens=0; entradas=0
            for l in linhas:
                if l[0]=='ENTRA':
                    assert len(l)==2; i=int(l[1])
                    assert fase=='entrada' and 0<=i<p['pessoas'] and i not in dentro and len(dentro)<p['capacidade']
                    assert not p.get('unicos',False) or i not in usados
                    dentro.add(i);usados.add(i);entradas+=1
                elif l==['INICIO']:
                    assert fase=='entrada' and len(dentro)==p['capacidade'];fase='viagem'
                elif l==['FIM']:
                    assert fase=='viagem';viagens+=1
                    if p['saida']:fase='saida'
                    else:dentro.clear();fase='entrada'
                else:
                    assert len(l)==2 and l[0]=='SAI' and fase=='saida';i=int(l[1]);assert i in dentro
                    dentro.remove(i)
                    if not dentro:fase='entrada'
            return not dentro and fase=='entrada' and viagens==p['viagens'] and entradas==p['capacidade']*viagens
        if tipo=='comida':
            estoque=p['capacidade'];cont=[0]*p['pessoas']
            for l in linhas:
                if l==['REPOE']:
                    assert estoque==0 and sum(cont)<p['pessoas']*p['refeicoes'];estoque=p['capacidade']
                else:
                    assert len(l)==2 and l[0]=='COME';i=int(l[1]);assert 0<=i<len(cont) and estoque>0
                    estoque-=1;cont[i]+=1;assert cont[i]<=p['refeicoes']
            return cont==[p['refeicoes']]*p['pessoas']
        if tipo=='recurso':
            ativos=set();cont=[0]*p['pessoas']
            for l in linhas:
                assert len(l)==2;i=int(l[1]);assert 0<=i<len(cont)
                if l[0]=='IN':
                    assert i not in ativos and len(ativos)<p['capacidade'] and cont[i]<p['vezes']
                    if p.get('vizinhos'):assert (i-1)%len(cont) not in ativos and (i+1)%len(cont) not in ativos
                    ativos.add(i)
                else:
                    assert l[0]=='OUT' and i in ativos;ativos.remove(i);cont[i]+=1
            return not ativos and cont==[p['vezes']]*len(cont)
        if tipo=='barreira':
            chega=[set() for _ in range(p['rodadas'])];passa=[set() for _ in chega]
            for l in linhas:
                assert len(l)==3;r,i=map(int,l[1:]);assert 0<=r<len(chega) and 0<=i<p['pessoas']
                if l[0]=='CHEGA':
                    assert i not in chega[r] and (r==0 or i in passa[r-1]);chega[r].add(i)
                else:
                    assert l[0]=='PASSA' and len(chega[r])==p['pessoas'] and i not in passa[r];passa[r].add(i)
            return all(len(x)==p['pessoas'] for x in passa)
        if tipo=='rw':
            leitores=set();escritor=None;valor=0;rc=[0]*p['leitores'];wc=[0]*p['escritores']
            for l in linhas:
                assert len(l)>=2;i=int(l[1])
                if l[0]=='RI':
                    assert len(l)==3 and 0<=i<len(rc) and i not in leitores and escritor is None and int(l[2])==valor
                    assert rc[i]<p['vezes'];leitores.add(i)
                elif l[0]=='RF':
                    assert len(l)==2 and i in leitores;leitores.remove(i);rc[i]+=1
                elif l[0]=='WI':
                    assert len(l)==2 and 0<=i<len(wc) and escritor is None and not leitores and wc[i]<p['vezes'];escritor=i
                else:
                    assert l[0]=='WF' and len(l)==3 and escritor==i and int(l[2])==valor+1
                    valor+=1;wc[i]+=1;escritor=None
            return not leitores and escritor is None and rc==[p['vezes']]*len(rc) and wc==[p['vezes']]*len(wc)
        if tipo=='porto':
            estados=['novo']*p['pessoas'];ocupacao=0;canal=None;saidas=set()
            for l in linhas:
                assert len(l)==2;i=int(l[1]);assert 0<=i<len(estados);op=l[0]
                if op=='PE':assert estados[i]=='novo';estados[i]='espera_entrada'
                elif op=='EN':
                    assert estados[i]=='espera_entrada' and canal is None and not saidas and ocupacao<p['capacidade']
                    ocupacao+=1;canal=i;estados[i]='entrando'
                elif op=='AT':
                    assert estados[i]=='entrando' and canal==i;canal=None;estados[i]='porto'
                elif op=='PS':assert estados[i]=='porto';saidas.add(i);estados[i]='espera_saida'
                elif op=='SA':
                    assert estados[i]=='espera_saida' and canal is None;saidas.remove(i);canal=i;estados[i]='saindo'
                else:
                    assert op=='FI' and estados[i]=='saindo' and canal==i;canal=None;ocupacao-=1;estados[i]='fim'
            return estados==['fim']*len(estados) and canal is None and ocupacao==0
        if tipo=='barbearia':
            esperado=[];cadeira=None;fila=[]
            for op in p['operacoes']:
                x=op.split()
                if x[0]=='A':
                    i=int(x[1])
                    if cadeira is None:cadeira=i;esperado.append(['ATENDE',str(i)])
                    elif len(fila)<p['cadeiras']:fila.append(i);esperado.append(['ESPERA',str(i)])
                    else:esperado.append(['RECUSA',str(i)])
                else:
                    if cadeira is None:esperado.append(['LIVRE'])
                    else:
                        esperado.append(['FIM',str(cadeira)])
                        cadeira=fila.pop(0) if fila else None
                        esperado.append(['ATENDE',str(cadeira)] if cadeira is not None else ['LIVRE'])
            return linhas==esperado and cadeira is None and not fila
        return False
    except (AssertionError, ValueError, KeyError, IndexError, TypeError):
        return False
