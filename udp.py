import socket
import threading
import sys

# --- CONFIGURACIÓN ---
IP_DESTINO = "127.0.0.1"  # IP del otro dispositivo
PUERTO_DESTINO = 5005
PUERTO_LOCAL = 5006    # Puerto donde nosotros escuchamos

def formatear_bytes(datos):
    """Convierte bytes a formato 0x22, 0x66, ..."""
    return ", ".join([f"0x{b:02X}" for b in datos])

def hilo_receptor(sock):
    """Función que corre en segundo plano recibiendo datos."""
    print(f"[*] Receptor iniciado en el puerto {PUERTO_LOCAL}...")
    while True:
        try:
            data, addr = sock.recvfrom(1024)
            print(f"\n[NOTICIA] Mensaje recibido de {addr}:")
            print(f"    Bytes: {formatear_bytes(data)}")
            print(">> Escribe bytes en hex (ej: 22, 66, A3) o 'salir': ", end="")
        except Exception as e:
            # Si el socket se cierra, salimos del bucle
            break

def udp_interactive_client():
    # Crear socket UDP
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    
    # Enlazar a nuestro puerto local para poder RECIBIR
    sock.bind(("0.0.0.0", PUERTO_LOCAL))

    # Iniciar el hilo de recepción
    thread = threading.Thread(target=hilo_receptor, args=(sock,), daemon=True)
    thread.start()

    print("=== CLIENTE UDP HEX INTERACTIVO ===")
    print(f"Destino configurado: {IP_DESTINO}:{PUERTO_DESTINO}")
    print("Instrucciones: Escribe los bytes separados por comas o espacios.")
    print("Ejemplo: 22, 66, A3")
    print("-" * 35)

    try:
        while True:
            entrada = input(">> Escribe bytes en hex o 'salir': ").strip().lower()
            
            if entrada == 'salir':
                break
            
            if not entrada:
                continue

            try:
                # Limpiar la entrada (quitar 0x, comas, espacios)
                limpia = entrada.replace("0x", "").replace(",", " ").split()
                # Convertir strings hex a lista de enteros
                lista_bytes = [int(x, 16) for x in limpia]
                trama = bytes(lista_bytes)
                
                # Enviar
                sock.sendto(trama, (IP_DESTINO, PUERTO_DESTINO))
                print(f"[OK] Enviados {len(trama)} bytes.")
                
            except ValueError:
                print("[ERROR] Formato hexadecimal inválido. Usa: 22, 66, A3")

    except KeyboardInterrupt:
        pass
    finally:
        print("\nCerrando socket y saliendo...")
        sock.close()

if __name__ == "__main__":
    udp_interactive_client()
