import matplotlib.pyplot as plt
import numpy as np

def ler_dados_cpu(arquivo="linha_tempo_cpu.txt"):
    """Lê o arquivo de linha do tempo da CPU"""
    tempos = []
    processos = []

    try:
        with open(arquivo, 'r') as f:
            for linha in f:
                linha = linha.strip()
                if not linha:
                    continue
                t, p = map(int, linha.split())
                tempos.append(t)
                processos.append(p)
        return tempos, processos
    except FileNotFoundError:
        print(f"Erro: Arquivo '{arquivo}' não encontrado!")
        print("Execute a simulação primeiro para gerar o arquivo.")
        exit(1)

def criar_grafico_cpu(tempos, processos):
    """Cria o gráfico de ocupação da CPU com linhas horizontais"""


    processos_unicos = sorted(set(processos))
    processos_sem_ocioso = [p for p in processos_unicos if p != 0]

    print(f"Processos encontrados: {processos_sem_ocioso}")
    print(f"Total de processos: {len(processos_sem_ocioso)}")


    y_posicoes = {0: 0}


    for i, p in enumerate(processos_sem_ocioso, start=1):
        y_posicoes[p] = i


    tempos_step = tempos.copy()
    processos_step = processos.copy()
    tempos_step.append(tempos[-1] + 1)
    processos_step.append(processos[-1])

    y_valores = [y_posicoes[p] for p in processos_step]


    fig, ax = plt.subplots(figsize=(14, 6))


    for i in range(len(tempos_step) - 1):
        x_inicio = tempos_step[i]
        x_fim = tempos_step[i + 1]
        y = y_valores[i]


        ax.plot([x_inicio, x_fim], [y, y], color='black', linewidth=2.5)

        if i < len(tempos_step) - 2 and y_valores[i] != y_valores[i + 1]:
            ax.plot([x_fim, x_fim], [y, y_valores[i + 1]],
                    color='gray', linewidth=1.5, linestyle='--', alpha=0.7)


    

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


    legend_elements = [
        plt.Line2D([0], [0], color='black', linewidth=2.5, label='Processo na CPU'),
        plt.Line2D([0], [0], color='gray', linewidth=1.5, linestyle='--', label='Mudança de processo'),
        plt.Line2D([0], [0], marker='o', color='red', markersize=8,
                   linestyle='None', label='Ponto de mudança')
    ]
    ax.legend(handles=legend_elements, loc='upper right', fontsize=10)


    plt.tight_layout()
    plt.savefig('cpu_ocupacao.png', dpi=150, bbox_inches='tight')
    print("Gráfico salvo como 'cpu_ocupacao.png'")

    plt.show()

def main():

    tempos, processos = ler_dados_cpu()

    criar_grafico_cpu(tempos, processos)

if __name__ == "__main__":
    main()