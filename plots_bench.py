import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys
import os

def main():
    if len(sys.argv) < 5:
        print("Ошибка: Укажите все аргументы")
        print("Пример: python plot.py cycles.txt scalar output.png stats.txt")
        print("  cycles.txt - файл с количеством тактов (одно число на строку)")
        print("  scalar - файл со статистикой (scalar/intrin/и т.д.)")
        print("  output.png - файл для сохранения графика")
        print("  stats.txt - файл для сохранения статистики")
        sys.exit(1)
    
    cycles_file = sys.argv[1]
    stats_file = sys.argv[2]
    output_file = sys.argv[3]
    stats_output = sys.argv[4]
    
    # Проверяем существование входных файлов
    if not os.path.exists(cycles_file):
        print(f"Ошибка: Файл '{cycles_file}' не найден!")
        sys.exit(1)
    
    if not os.path.exists(stats_file):
        print(f"Ошибка: Файл статистики '{stats_file}' не найден!")
        sys.exit(1)
    
    # Создаем директорию для выходного файла если нужно
    output_dir = os.path.dirname(output_file)
    if output_dir and not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"Создана папка: {output_dir}")
    
    # Создаем директорию для файла статистики если нужно
    stats_dir = os.path.dirname(stats_output)
    if stats_dir and not os.path.exists(stats_dir):
        os.makedirs(stats_dir)
    
    # === 1. Читаем данные тактов из файла ===
    cycles = []
    with open(cycles_file, 'r') as f:
        for line in f:
            line = line.strip()
            if line:
                cycles.append(int(line))
    
    cycles = np.array(cycles)
    cycles_iterations = np.arange(1, len(cycles) + 1)
    
    # Считаем статистику для тактов
    cycles_mean = np.mean(cycles)
    cycles_std = np.std(cycles)
    
    # === 2. Читаем данные статистики ===
    df_stats = pd.read_csv(stats_file, sep='\t')
    print(f"Загружен файл статистики: {stats_file}")
    
    # Получаем имя файла статистики для подписи
    stats_name = os.path.basename(stats_file)
    
    # Считаем статистику для частоты
    bzy_mean = df_stats['Bzy_MHz'].mean()
    bzy_std = df_stats['Bzy_MHz'].std()
    
    # Создаем время для статистики (шаг 0.5 секунды)
    TIME_STEP = 0.5
    df_stats['Time_sec'] = np.arange(0, len(df_stats) * TIME_STEP, TIME_STEP)[:len(df_stats)]
    
    # === Записываем статистику в файл ===
    with open(stats_output, 'a') as f:
        f.write(f"{stats_name}: {cycles_mean/1e8:.2f}±{3*cycles_std/1e8:.2f} ×10⁸\n")
    
    print(f"Статистика добавлена в: {stats_output}")
    
    # === Создаем фигуру с двумя подграфиками ===
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(16, 9))
    
    # Расстояние между графиками
    plt.subplots_adjust(hspace=0.3)
    
    # === ГРАФИК 1: Такты от итерации ===
    ax1.plot(cycles_iterations, cycles, 'b-', linewidth=0.5, label='Такты')
    ax1.axhline(y=cycles_mean, color='red', linestyle='--', linewidth=1.5, label=f'Среднее: {cycles_mean:.2e}')
    
    ax1.set_xlabel('Номер итерации', fontsize=11)
    ax1.set_ylabel('Количество тактов', fontsize=11)
    
    # Форматируем числа для заголовка
    mean_str = f'{cycles_mean/1e8:.2f}'
    std_str = f'{cycles_std/1e8:.2f}'
    sigma3_str = f'{3*cycles_std/1e8:.2f}'
    ax1.set_title(f'Такты: среднее = {mean_str}×10⁸, σ = {std_str}×10⁸, 3σ = {sigma3_str}×10⁸ ({stats_name})', 
                  fontsize=12, fontweight='bold')
    
    ax1.grid(True, alpha=0.3)
    ax1.legend(loc='upper right', fontsize=9)
    ax1.ticklabel_format(axis='y', style='scientific', scilimits=(8, 8))
    ax1.yaxis.get_offset_text().set_fontsize(10)
    
    # === ГРАФИК 2: Частота и температура ===
    bar_width = TIME_STEP * 0.35  # Уменьшена ширина для отступов между барами
    
    # Левая ось - частота Bzy_MHz
    ax2.bar(df_stats['Time_sec'] - bar_width/2, df_stats['Bzy_MHz'], 
            width=bar_width, 
            color='steelblue', 
            edgecolor='navy', 
            linewidth=0.5,
            alpha=0.6,
            label='Bzy_MHz')
    
    ax2.axhline(y=bzy_mean, color='darkblue', linestyle='--', linewidth=1.5, label=f'Среднее: {bzy_mean:.1f}')
    
    ax2.set_xlabel('Время (секунды)', fontsize=11)
    ax2.set_ylabel('Частота (MHz)', fontsize=11, color='steelblue')
    ax2.tick_params(axis='y', labelcolor='steelblue')
    ax2.grid(True, alpha=0.3, axis='y')
    ax2.set_ylim(2390, 2405)
    
    # Правая ось - температура
    ax2_twin = ax2.twinx()
    ax2_twin.bar(df_stats['Time_sec'] + bar_width/2, df_stats['CoreTmp'], 
                 width=bar_width, 
                 color='coral', 
                 edgecolor='darkred', 
                 linewidth=0.5,
                 alpha=0.6,
                 label='CoreTmp')
    
    ax2_twin.set_ylabel('Температура (°C)', fontsize=11, color='coral')
    ax2_twin.tick_params(axis='y', labelcolor='coral')
    ax2_twin.set_ylim(40, 55)
    
    # Форматируем заголовок с частотой
    ax2.set_title(f'Частота: среднее = {bzy_mean:.1f} MHz, σ = {bzy_std:.2f} MHz, 3σ = {3*bzy_std:.2f} MHz ({stats_name})', 
                  fontsize=12, fontweight='bold')
    
    # Настройка оси X для графика статистики
    max_time = df_stats['Time_sec'].iloc[-1]
    if max_time <= 30:
        interval = 5
    elif max_time <= 60:
        interval = 10
    else:
        interval = 15
    
    xticks = np.arange(0, max_time + interval, interval)
    ax2.set_xticks(xticks)
    ax2.set_xticklabels([f'{x:.1f}' for x in xticks], rotation=45)
    ax2.set_xlim(-TIME_STEP, max_time + TIME_STEP)
    
    # Легенда
    lines1, labels1 = ax2.get_legend_handles_labels()
    lines2, labels2 = ax2_twin.get_legend_handles_labels()
    ax2.legend(lines1 + lines2, labels1 + labels2, loc='upper right', fontsize=9)
    
    ax2_twin.grid(False)
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=300, bbox_inches='tight')
    print(f"\nГрафик сохранен: {output_file}")
    # plt.show()  # закомментировано

if __name__ == "__main__":
    main()
