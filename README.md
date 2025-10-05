# Basquet_App 🏀

Aplicación de escritorio para gestión de equipos de baloncesto desarrollada con Qt6 y PostgreSQL.

## 🎯 Descripción

Sistema integral para la administración de:
- **Atletas y Entrenadores**: Registro y gestión de personas
- **Equipos**: Formación y composición de equipos por categorías
- **Partidos**: Registro de encuentros y resultados
- **Autenticación**: Sistema de login con control de sesiones
- **Permisos**: Control de acceso basado en roles (RBAC)

## 🛠️ Tecnologías

- **Frontend**: Qt6 Widgets (C++17)
- **Backend**: PostgreSQL con libpq
- **Build System**: CMake 3.16+
- **Arquitectura**: MVC (Model-View-Controller)

## 📋 Requisitos

### Sistema
```bash
# Debian/Ubuntu
sudo apt install build-essential cmake
sudo apt install qt6-base-dev qt6-tools-dev
sudo apt install postgresql postgresql-contrib libpq-dev

# Arch Linux
sudo pacman -S base-devel cmake
sudo pacman -S qt6-base qt6-tools
sudo pacman -S postgresql postgresql-libs
```

### Configuración de PostgreSQL
```bash
# Iniciar servicio
sudo systemctl start postgresql
sudo systemctl enable postgresql

# Crear base de datos
sudo -u postgres psql
CREATE DATABASE basket_app;
CREATE USER jschavarria WITH PASSWORD 'segmenttree';
GRANT ALL PRIVILEGES ON DATABASE basket_app TO jschavarria;
\q
```

## 🚀 Instalación

### 1. Clonar el repositorio
```bash
git clone https://github.com/profetaRecursivo/basket_app_tbd.git
cd basket_app_tbd
```

### 2. Configurar la base de datos
```bash
# Ejecutar el script DDL
psql -U jschavarria -d basket_app -f docs/database_schema.sql
```

### 3. Compilar el proyecto
```bash
mkdir -p build/Desktop-Debug
cd build/Desktop-Debug
cmake ../..
make
```

### 4. Ejecutar
```bash
./Basquet_App
```

## 📁 Estructura del Proyecto

```
Basquet_App/
├── src/
│   ├── main.cpp                    # Punto de entrada
│   ├── models/
│   │   └── Auth/
│   │       ├── AuthManager.h/cpp   # Gestión de autenticación
│   │       └── DbManager.h/cpp     # Conexión a PostgreSQL
│   ├── views/
│   │   └── login/
│   │       └── LoginWindow.h/cpp   # Ventana de login
│   └── controllers/                # Controladores (vacío por ahora)
├── build/                          # Archivos de compilación
├── docs/
│   └── DATABASE_SCHEMA.md         # Documentación de la BD
├── CMakeLists.txt                 # Configuración de CMake
└── README.md                      # Este archivo
```

## 🔐 Credenciales de Prueba

```
Usuario: pollo_primo
Contraseña: economico123
```

## 🏗️ Arquitectura

### Patrón MVC
```
┌─────────────┐
│    Views    │ ← Qt Widgets (LoginWindow)
└──────┬──────┘
       │
┌──────▼──────────┐
│  Controllers    │ ← Lógica de negocio (por implementar)
└──────┬──────────┘
       │
┌──────▼──────┐
│   Models    │ ← AuthManager, DbManager
└──────┬──────┘
       │
┌──────▼──────┐
│ PostgreSQL  │ ← Base de datos
└─────────────┘
```

### Sistema de Autenticación
1. Usuario ingresa credenciales en `LoginWindow`
2. Se emite señal `loginRequested(user, pass)`
3. `AuthManager::validate()` consulta la BD
4. Se obtiene el `id_userN` con `get_user_id()`
5. Se registra la sesión con `insert_session()`
6. Se retorna el código de estado (1: éxito, 0: credenciales incorrectas, -1: error BD)

## 📚 Documentación Adicional

- [Esquema de Base de Datos](docs/DATABASE_SCHEMA.md) - Estructura completa de tablas y relaciones

## 🎯 Estado Actual

### ✅ Implementado
- Sistema de login funcional
- Conexión a PostgreSQL
- Gestión de sesiones con PID
- Validación de credenciales

### 🚧 En Desarrollo
- Ventana principal (MainWindow)
- Sistema de navegación por permisos
- CRUD de personas
- CRUD de atletas
- CRUD de equipos

### 📝 Pendiente
- Gestión de partidos
- Reportes y estadísticas
- Sistema completo RBAC
- Gestión de categorías y roles

## 🤝 Contribuir

1. Fork el proyecto
2. Crea una rama para tu feature (`git checkout -b feature/nueva-funcionalidad`)
3. Commit tus cambios (`git commit -m 'Agregar nueva funcionalidad'`)
4. Push a la rama (`git push origin feature/nueva-funcionalidad`)
5. Abre un Pull Request

## 👨‍💻 Autor

**@profetaRecursivo**

## 📄 Licencia

Este proyecto es parte de un trabajo académico para TBD (Taller de Base de Datos).

## 🔧 Configuración de VS Code

Si usas VS Code, asegúrate de tener instaladas las siguientes extensiones:
- C/C++ (Microsoft)
- CMake Tools
- Qt tools

El proyecto incluye configuración de IntelliSense para Qt6 y PostgreSQL en `.vscode/c_cpp_properties.json`.
