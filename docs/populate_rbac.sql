-- ============================================
-- Script de Población RBAC - Basquet App
-- PostgreSQL 14
-- ============================================
-- Este script inserta los roles, funciones, UIs
-- y sus relaciones para el sistema RBAC
-- ============================================

-- ============================================
-- 1. INSERTAR ROLES (3 roles)
-- ============================================
INSERT INTO public."RolDB" (nombre) VALUES
('ADMIN'),
('ENTRENADOR'),
('ATLETA')
ON CONFLICT DO NOTHING;

-- ============================================
-- 2. INSERTAR UIs (8 interfaces)
-- ============================================
INSERT INTO public."UI" (nombre) VALUES
('UI_LOGIN'),
('UI_DASHBOARD'),
('UI_GESTION_ATLETAS'),
('UI_MI_PERFIL'),
('UI_GESTION_EQUIPOS'),
('UI_GESTION_MIEMBROS'),
('UI_MI_EQUIPO'),
('UI_GESTION_PARTIDOS')
ON CONFLICT DO NOTHING;

-- ============================================
-- 3. INSERTAR FUNCIONES (12 funciones)
-- ============================================
INSERT INTO public."Funcion" (nombre) VALUES
-- Módulo Atletas
('FN_GESTIONAR_ATLETAS'),
('FN_VER_ATLETAS'),
('FN_VER_MI_PERFIL'),
-- Módulo Equipos
('FN_GESTIONAR_EQUIPOS'),
('FN_GESTIONAR_MIEMBROS'),
('FN_VER_EQUIPOS'),
('FN_VER_MI_EQUIPO'),
-- Módulo Partidos
('FN_GESTIONAR_PARTIDOS'),
('FN_GESTIONAR_RESULTADOS'),
('FN_VER_PARTIDOS'),
('FN_VER_MIS_PARTIDOS'),
-- Dashboard
('FN_VER_DASHBOARD')
ON CONFLICT DO NOTHING;

-- ============================================
-- 4. ASIGNAR FUNCIONES A ROLES
-- ============================================

-- 4.1. ROL ADMIN: Todas las funciones
INSERT INTO public."Rol_Funcion" (id_rol, id_funcion, active)
SELECT 
    (SELECT id_rol FROM public."RolDB" WHERE nombre = 'ADMIN'),
    id_funcion,
    true
FROM public."Funcion"
ON CONFLICT DO NOTHING;

-- 4.2. ROL ENTRENADOR: Solo funciones de VER
INSERT INTO public."Rol_Funcion" (id_rol, id_funcion, active)
SELECT 
    (SELECT id_rol FROM public."RolDB" WHERE nombre = 'ENTRENADOR'),
    id_funcion,
    true
FROM public."Funcion"
WHERE nombre IN (
    'FN_VER_ATLETAS', 
    'FN_VER_EQUIPOS', 
    'FN_VER_PARTIDOS', 
    'FN_VER_DASHBOARD'
)
ON CONFLICT DO NOTHING;

-- 4.3. ROL ATLETA: Solo funciones personales
INSERT INTO public."Rol_Funcion" (id_rol, id_funcion, active)
SELECT 
    (SELECT id_rol FROM public."RolDB" WHERE nombre = 'ATLETA'),
    id_funcion,
    true
FROM public."Funcion"
WHERE nombre IN (
    'FN_VER_MI_PERFIL', 
    'FN_VER_MI_EQUIPO', 
    'FN_VER_MIS_PARTIDOS', 
    'FN_VER_DASHBOARD'
)
ON CONFLICT DO NOTHING;

-- ============================================
-- 5. ASIGNAR UIs A FUNCIONES
-- ============================================

-- FN_GESTIONAR_ATLETAS → UI_GESTION_ATLETAS
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_GESTIONAR_ATLETAS'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_GESTION_ATLETAS'),
    true
ON CONFLICT DO NOTHING;

-- FN_VER_ATLETAS → UI_GESTION_ATLETAS
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_VER_ATLETAS'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_GESTION_ATLETAS'),
    true
ON CONFLICT DO NOTHING;

-- FN_VER_MI_PERFIL → UI_MI_PERFIL
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_VER_MI_PERFIL'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_MI_PERFIL'),
    true
ON CONFLICT DO NOTHING;

-- FN_GESTIONAR_EQUIPOS → UI_GESTION_EQUIPOS
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_GESTIONAR_EQUIPOS'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_GESTION_EQUIPOS'),
    true
ON CONFLICT DO NOTHING;

-- FN_VER_EQUIPOS → UI_GESTION_EQUIPOS
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_VER_EQUIPOS'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_GESTION_EQUIPOS'),
    true
ON CONFLICT DO NOTHING;

-- FN_GESTIONAR_MIEMBROS → UI_GESTION_MIEMBROS
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_GESTIONAR_MIEMBROS'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_GESTION_MIEMBROS'),
    true
ON CONFLICT DO NOTHING;

-- FN_VER_MI_EQUIPO → UI_MI_EQUIPO
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_VER_MI_EQUIPO'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_MI_EQUIPO'),
    true
ON CONFLICT DO NOTHING;

-- FN_GESTIONAR_PARTIDOS → UI_GESTION_PARTIDOS
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_GESTIONAR_PARTIDOS'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_GESTION_PARTIDOS'),
    true
ON CONFLICT DO NOTHING;

-- FN_GESTIONAR_RESULTADOS → UI_GESTION_PARTIDOS
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_GESTIONAR_RESULTADOS'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_GESTION_PARTIDOS'),
    true
ON CONFLICT DO NOTHING;

-- FN_VER_PARTIDOS → UI_GESTION_PARTIDOS
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_VER_PARTIDOS'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_GESTION_PARTIDOS'),
    true
ON CONFLICT DO NOTHING;

-- FN_VER_MIS_PARTIDOS → UI_GESTION_PARTIDOS
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_VER_MIS_PARTIDOS'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_GESTION_PARTIDOS'),
    true
ON CONFLICT DO NOTHING;

-- FN_VER_DASHBOARD → UI_DASHBOARD
INSERT INTO public."Funcion_UI" (id_funcion, "id_UI", active)
SELECT 
    (SELECT id_funcion FROM public."Funcion" WHERE nombre = 'FN_VER_DASHBOARD'),
    (SELECT "id_UI" FROM public."UI" WHERE nombre = 'UI_DASHBOARD'),
    true
ON CONFLICT DO NOTHING;

-- ============================================
-- 6. CREAR USUARIOS DE PRUEBA
-- ============================================

-- Usuario ADMIN
INSERT INTO public."UserN" ("user", password) 
VALUES ('admin', 'admin123')
ON CONFLICT DO NOTHING;

-- Usuario ENTRENADOR (actualiza el existente si es necesario)
INSERT INTO public."UserN" ("user", password) 
VALUES ('entrenador', 'entrenador123')
ON CONFLICT DO NOTHING;

-- Usuario ATLETA
INSERT INTO public."UserN" ("user", password) 
VALUES ('atleta', 'atleta123')
ON CONFLICT DO NOTHING;

-- ============================================
-- 7. ASIGNAR ROLES A USUARIOS
-- ============================================

-- Asignar rol ADMIN al usuario 'admin'
INSERT INTO public."UserN_RolDB" ("id_userN", id_rol, active)
SELECT 
    (SELECT "id_userN" FROM public."UserN" WHERE "user" = 'admin'),
    (SELECT id_rol FROM public."RolDB" WHERE nombre = 'ADMIN'),
    true
WHERE EXISTS (SELECT 1 FROM public."UserN" WHERE "user" = 'admin')
  AND EXISTS (SELECT 1 FROM public."RolDB" WHERE nombre = 'ADMIN')
ON CONFLICT DO NOTHING;

-- Asignar rol ENTRENADOR al usuario 'entrenador'
INSERT INTO public."UserN_RolDB" ("id_userN", id_rol, active)
SELECT 
    (SELECT "id_userN" FROM public."UserN" WHERE "user" = 'entrenador'),
    (SELECT id_rol FROM public."RolDB" WHERE nombre = 'ENTRENADOR'),
    true
WHERE EXISTS (SELECT 1 FROM public."UserN" WHERE "user" = 'entrenador')
  AND EXISTS (SELECT 1 FROM public."RolDB" WHERE nombre = 'ENTRENADOR')
ON CONFLICT DO NOTHING;

-- Asignar rol ATLETA al usuario 'atleta'
INSERT INTO public."UserN_RolDB" ("id_userN", id_rol, active)
SELECT 
    (SELECT "id_userN" FROM public."UserN" WHERE "user" = 'atleta'),
    (SELECT id_rol FROM public."RolDB" WHERE nombre = 'ATLETA'),
    true
WHERE EXISTS (SELECT 1 FROM public."UserN" WHERE "user" = 'atleta')
  AND EXISTS (SELECT 1 FROM public."RolDB" WHERE nombre = 'ATLETA')
ON CONFLICT DO NOTHING;

-- ============================================
-- 8. VERIFICACIÓN (Consultas opcionales)
-- ============================================

-- Ver todos los roles
SELECT * FROM public."RolDB" ORDER BY id_rol;

-- Ver todas las funciones
SELECT * FROM public."Funcion" ORDER BY id_funcion;

-- Ver todas las UIs
SELECT * FROM public."UI" ORDER BY "id_UI";

-- Ver relación Rol → Funciones
SELECT 
    r.nombre AS rol,
    f.nombre AS funcion,
    rf.active
FROM public."Rol_Funcion" rf
JOIN public."RolDB" r ON rf.id_rol = r.id_rol
JOIN public."Funcion" f ON rf.id_funcion = f.id_funcion
ORDER BY r.nombre, f.nombre;

-- Ver relación Funcion → UIs
SELECT 
    f.nombre AS funcion,
    u.nombre AS ui,
    fu.active
FROM public."Funcion_UI" fu
JOIN public."Funcion" f ON fu.id_funcion = f.id_funcion
JOIN public."UI" u ON fu."id_UI" = u."id_UI"
ORDER BY f.nombre;

-- Ver usuarios con sus roles
SELECT 
    u."user" AS usuario,
    r.nombre AS rol,
    ur.active AS activo
FROM public."UserN_RolDB" ur
JOIN public."UserN" u ON ur."id_userN" = u."id_userN"
JOIN public."RolDB" r ON ur.id_rol = r.id_rol
ORDER BY u."user";

-- ============================================
-- RESUMEN DE USUARIOS CREADOS:
-- ============================================
-- usuario: admin      | password: admin123       | rol: ADMIN
-- usuario: entrenador | password: entrenador123  | rol: ENTRENADOR  
-- usuario: atleta     | password: atleta123      | rol: ATLETA
-- usuario: pollo_primo| password: economico123   | rol: (sin asignar aún)
-- ============================================
