# 🎮 MODO MULTIPLAYER - Polícia vs Ladrão

## ✨ Novidades

Agora o jogo suporta de **2 a 4 jogadores simultâneos**!

---

## 🕹️ MODOS DE JOGO

### 🎯 Menu Inicial
Ao iniciar, você verá um menu para selecionar o número de jogadores:

- **[1]** - 2 Jogadores (1 Policial vs 1 Ladrão) - *Modo Clássico*
- **[2]** - 3 Jogadores (2 Policiais vs 1 Ladrão)
- **[3]** - 4 Jogadores (3 Policiais vs 1 Ladrão)

---

## 🎮 CONTROLES

### 🦹 **LADRÃO** (sempre presente)
- **W** - Cima
- **A** - Esquerda
- **S** - Baixo
- **D** - Direita

### 👮 **POLICIAL 1** (sempre presente)
- **↑** - Cima
- **←** - Esquerda
- **↓** - Baixo
- **→** - Direita

### 👮 **POLICIAL 2** (apenas no modo 3-4 jogadores)
- **I** - Cima
- **J** - Esquerda
- **K** - Baixo
- **L** - Direita

### 👮 **POLICIAL 3** (apenas no modo 4 jogadores)
- **T** - Cima
- **F** - Esquerda
- **G** - Baixo
- **H** - Direita

---

## 🏆 REGRAS DO JOGO

### ⏱️ Objetivo
- **Ladrão**: Sobreviver por 180 segundos (3 minutos)
- **Policiais**: Capturar o ladrão antes do tempo acabar

### 🎯 Vitória
- **Policiais vencem** se QUALQUER policial encostar no ladrão
- **Ladrão vence** se o cronômetro chegar a 0

### 📊 Pontuação
- **Policial**: Score baseado na rapidez da captura
  - Quanto mais rápido, maior o score
  - Fórmula: `(180 - tempo_restante) × 10`
- **Ladrão**: Score fixo de 1800 pontos se sobreviver

### 🏅 Rankings Separados
- Ranking de **melhores capturas** (policiais)
- Ranking de **melhores fugas** (ladrões)
- Top 10 de cada categoria

---

## 🎨 INTERFACE VISUAL

### Durante o Jogo
- Cada policial tem uma **cor diferente** e um **número** (P1, P2, P3)
- Cronômetro no topo mostrando tempo restante
- Contador de jogadores ativos

### Tela de Fim
- Mostra qual policial venceu (se capturou)
- Pede o nome do vencedor
- Exibe ambos rankings lado a lado:
  - **Esquerda**: Ranking Policial (dourado)
  - **Direita**: Ranking Ladrão (verde)

---

## 🚀 COMO JOGAR

### Compilar
```bash
cd /home/mfpca/Jogo
make clean
make
```

### Executar
```bash
./RunnerC
```

### Fluxo do Jogo
1. **Menu** - Escolha número de jogadores (1, 2 ou 3)
2. **Gameplay** - Jogue até captura ou tempo esgotar
3. **Entrada de Nome**:
   - Digite seu nome e pressione **ENTER**
   - Ou pressione **ESPAÇO** para salvar como "ANON"
4. **Rankings** - Veja os top 10 de cada categoria
5. **Reiniciar** - Pressione **ENTER** para voltar ao menu

---

## 📁 ARQUIVOS DE RANKING

Os rankings são salvos automaticamente em:
- `ranking_policia.txt` - Top 10 capturas mais rápidas
- `ranking_ladrao.txt` - Top 10 fugas bem-sucedidas

---

## 💡 DICAS ESTRATÉGICAS

### Para Policiais:
- **Modo 2 policiais**: Coordenem para cercar o ladrão
- **Modo 3 policiais**: Um bloqueia cada saída do corredor
- Comuniquem-se para não deixar rotas de fuga

### Para o Ladrão:
- Use os corredores estreitos a seu favor
- Fique em movimento constante
- Quanto mais policiais, mais difícil, mas mais glorioso!

---

## 🎉 DIVIRTA-SE!

Agora você pode jogar com seus amigos no mesmo teclado! 
Quem será o melhor policial? Quem conseguirá a fuga mais épica? 🏃‍♂️👮‍♀️
