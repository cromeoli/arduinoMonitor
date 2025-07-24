#!/usr/bin/env python3
import time
import psutil
import os

try:
    import serial
    print("✓ Módulo serial importado correctamente")
except ImportError as e:
    print(f"✗ Error importando serial: {e}")
    print("Instala con: pip install pyserial")
    exit(1)

# Verificar que el puerto existe
if not os.path.exists('/dev/ttyUSB0'):
    print("✗ Puerto /dev/ttyUSB0 no encontrado")
    print("Conecta el Arduino y verifica con: ls /dev/ttyUSB*")
    exit(1)

try:
    # Configurar puerto serie
    arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
    print("✓ Puerto serie conectado")
    time.sleep(2)
    
    while True:
        # Obtener temperatura CPU y GPU
        cpu_temp = 0
        gpu_temp = 0
        
        # Método 1: psutil para AMD
        try:
            temps = psutil.sensors_temperatures()
            if temps:
                # CPU AMD (k10temp)
                if 'k10temp' in temps:
                    cpu_temp = int(temps['k10temp'][0].current)
                
                # GPU AMD (amdgpu)
                if 'amdgpu' in temps:
                    gpu_temp = int(temps['amdgpu'][0].current)
        except:
            pass
        
        # Método 2: sensors command como respaldo
        if cpu_temp == 0:
            try:
                import subprocess
                result = subprocess.run(['sensors'], capture_output=True, text=True)
                lines = result.stdout.split('\n')
                for line in lines:
                    if 'Tctl:' in line:  # AMD CPU
                        import re
                        match = re.search(r'\+(\d+\.\d+)°C', line)
                        if match:
                            cpu_temp = int(float(match.group(1)))
                    elif 'edge:' in line:  # AMD GPU
                        import re
                        match = re.search(r'\+(\d+\.\d+)°C', line)
                        if match:
                            gpu_temp = int(float(match.group(1)))
            except:
                pass
        
        # Usar CPU como temperatura principal
        temp = cpu_temp if cpu_temp > 0 else 45
        
        # Obtener RAM
        ram_percent = int(psutil.virtual_memory().percent)
        
        # Enviar al Arduino
        data = f"TEMP:{temp},GPU:{gpu_temp},RAM:{ram_percent}\n"
        arduino.write(data.encode())
        
        print(f"Enviado: CPU {temp}°C, GPU {gpu_temp}°C, RAM {ram_percent}%")
        time.sleep(2)
        
except serial.SerialException as e:
    print(f"✗ Error de puerto serie: {e}")
    print("Verifica que el Arduino esté conectado y tengas permisos")
except KeyboardInterrupt:
    print("\n✓ Programa terminado por el usuario")
    arduino.close()
except Exception as e:
    print(f"✗ Error inesperado: {e}")