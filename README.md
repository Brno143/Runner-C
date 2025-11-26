# Runner-C

**Runner-C** é um jogo para dois jogadores, desenvolvido em C, para Linux, executado na linha de comando. O jogo é ambientado em um mapa estilo labirinto (semelhante ao Pac-Man), onde um jogador assume o papel de **policial** e o outro, o **ladrão**. Ambos os jogadores competem de maneira estratégica, usando power-ups e habilidades para alcançar seus objetivos. O jogo também conta com um sistema de ranking baseado no tempo, onde o objetivo é completar o jogo o mais rápido possível para alcançar uma posição alta.

---

## Objetivo

- **Ladrão**: O objetivo do **ladrão** é coletar todas as chaves espalhadas pelo mapa para escapar da "prisão" antes que o policial o capture.
- **Policial**: O objetivo do **policial** é capturar o ladrão antes que ele consiga coletar todas as chaves e escapar.

Ambos os jogadores podem usar power-ups durante o jogo, que incluem armadilhas, escudos e velocidade. Esses power-ups ajudam os jogadores a cumprir seus objetivos ou a dificultar as ações do adversário.

---

## Características

- **Modo de Jogo para 2 Jogadores**: Um jogador assume o papel do policial e o outro o do ladrão.
- **Mapa Estilo Labirinto**: Um mapa gerado aleatoriamente no estilo de labirinto, com paredes e caminhos que ambos os jogadores devem explorar.
- **Power-ups**:
  - **Armadilha**: Usado para dificultar a movimentação do oponente.
  - **Escudo**: Oferece proteção temporária contra armadilhas ou capturas.
  - **Velocidade**: Aumenta a velocidade de movimentação de um dos jogadores por um tempo limitado.
- **Ranking**: O desempenho de cada jogador é registrado com base no tempo de jogo. Quanto menor o tempo, melhor a posição no ranking.
  
---

## Como Jogar

1. **Início do Jogo**: Ao iniciar o jogo, o mapa será gerado aleatoriamente e as posições iniciais de ambos os jogadores serão determinadas.
2. **Controles**:
   - **Policial**: Usa as teclas de direção (`w`, `a`, `s`, `d`) para se mover pelo mapa e tentar capturar o ladrão.
   - **Ladrão**: Também usa as teclas de direção (`w`, `a`, `s`, `d`) para se mover pelo mapa e coletar as chaves espalhadas.
   - Ambos os jogadores podem usar os power-ups conforme encontrados no mapa.
3. **Objetivo**:
   - O **ladrão** deve coletar todas as chaves espalhadas pelo mapa.
   - O **policial** deve capturar o ladrão antes que ele consiga escapar.

4. **Vencer o Jogo**: O jogo termina quando:
   - O **ladrão** coleta todas as chaves e escapa.
   - O **policial** captura o ladrão.

5. **Ranking**: O tempo necessário para alcançar o objetivo será registrado e, quanto menor o tempo, mais alto será o ranking do jogador vencedor (seja o policial ou o ladrão).


