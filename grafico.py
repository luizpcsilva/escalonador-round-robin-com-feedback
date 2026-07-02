import matplotlib.pyplot as plt
import numpy as np
import matplotlib.ticker as ticker

def ler_dados_cpu(arquivo="linha_tempo_cpu.txt"):
    tempos = []
    processos = []
    terminados = []
    
    try:
        with open(arquivo, 'r') as f:
            for linha in f:
                linha = linha.strip()
                if not linha:
                    continue
                partes = linha.split()
                if len(partes) >= 3:
                    t, p, term = map(int, partes[:3])
                    tempos.append(t)
                    processos.append(p)
                    terminados.append(term)
                else:
                    # Fallback para formato antigo (2 colunas)
                    t, p = map(int, partes)
                    tempos.append(t)
                    processos.append(p)
                    terminados.append(0)
        return tempos, processos, terminados
    except FileNotFoundError:
        print(f"Erro: Arquivo '{arquivo}' não encontrado")
        print("Execute a simulação primeiro para gerar o arquivo.")
        exit(1)

def criar_grafico_cpu(tempos, processos, terminados):  
    processos_unicos = sorted(set(processos))
    processos_sem_ocioso = [p for p in processos_unicos if p != 0]
    
    print(f"Processos encontrados: {processos_sem_ocioso}")
    print(f"Total de processos: {len(processos_sem_ocioso)}")
    
    # Mapeia processos para posições Y
    y_posicoes = {0: 0}
    for i, p in enumerate(processos_sem_ocioso, start=1):
        y_posicoes[p] = i
    
    # Prepara dados para o step plot
    tempos_step = tempos.copy()
    processos_step = processos.copy()
    tempos_step.append(tempos[-1] + 1)
    processos_step.append(processos[-1])
    
    y_valores = [y_posicoes[p] for p in processos_step]
    
    fig, ax = plt.subplots(figsize=(14, 6))
    
    # Plota as linhas horizontais (step plot)
    for i in range(len(tempos_step) - 1):
        x_inicio = tempos_step[i]
        x_fim = tempos_step[i + 1]
        y = y_valores[i]
        
        # Linha horizontal
        ax.plot([x_inicio, x_fim], [y, y], color='black', linewidth=2.5)
        
        # Linha vertical no ponto de mudança (tracejada)
        if i < len(tempos_step) - 2 and y_valores[i] != y_valores[i + 1]:
            ax.plot([x_fim, x_fim], [y, y_valores[i + 1]], 
                    color='gray', linewidth=1.5, linestyle='--', alpha=0.7)
    
    # ===== MARCA APENAS O FIM DA EXECUÇÃO (terceira coluna) =====
    for i, term in enumerate(terminados):
        if term != 0 and term in y_posicoes:
            y = y_posicoes[term]
            # Bolinha vermelha no momento exato que o processo terminou
            ax.scatter(tempos[i], y, color='red', s=120, zorder=5, 
                      marker='o', edgecolor='darkred', linewidth=2)
    
    # Configuração dos eixos
    labels = ['Ocioso'] + [f'P{p}' for p in processos_sem_ocioso]
    y_ticks = list(range(len(labels)))
    ax.set_yticks(y_ticks)
    ax.set_yticklabels(labels)
    
    ax.set_xlabel('Tempo (unidades)', fontsize=12, fontweight='bold')
    ax.set_ylabel('Processo na CPU', fontsize=12, fontweight='bold')
    ax.set_title('Ocupação da CPU ao Longo do Tempo', fontsize=14, fontweight='bold')
    
    ax.grid(True, axis='x', linestyle='--', alpha=0.3)
    ax.set_xlim([min(tempos), max(tempos) + 1])
    ax.set_ylim([-0.5, len(labels) - 0.5])

    ax.xaxis.set_major_locator(ticker.MultipleLocator(10))
    plt.xticks(rotation=45)


    # Legenda
    legend_elements = [
        plt.Line2D([0], [0], color='black', linewidth=2.5, label='Processo na CPU'),
        plt.Line2D([0], [0], color='gray', linewidth=1.5, linestyle='--', label='Mudança de processo'),
        plt.Line2D([0], [0], marker='o', color='red', markersize=10, 
                   linestyle='None', label='Fim do processo')
    ]
    ax.legend(handles=legend_elements, loc='upper right', fontsize=10)
    
    plt.tight_layout()
    plt.savefig('cpu_ocupacao.png', dpi=150, bbox_inches='tight')
    print("Gráfico salvo como 'cpu_ocupacao.png'")
    
    plt.show()

def main():
    
    tempos, processos, terminados = ler_dados_cpu()
    
    print(f"   - Instantes: {len(tempos)}")
    print(f"   - Processos: {sorted(set(processos))}")
    print(f"   - Processos que terminaram: {[t for t in terminados if t != 0]}")
    
    criar_grafico_cpu(tempos, processos, terminados)

if __name__ == "__main__":
    main()