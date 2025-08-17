# RiveSprite2D - Guia de Uso

O **RiveSprite2D** é uma versão otimizada do RiveViewer que herda de Sprite2D, oferecendo todas as funcionalidades do Rive com melhor performance e integração nativa com o sistema de transformação do Godot.

## ✨ Principais Vantagens

### 🎯 Herança de Sprite2D
- **Transformação nativa**: Use `scale`, `rotation`, `position` normalmente
- **Compatibilidade**: Funciona com todos os sistemas que esperam Sprite2D
- **Facilidade de uso**: Interface familiar para desenvolvedores Godot

### ⚡ Otimizações de Performance
- **Frame skipping inteligente**: Evita renderização desnecessária
- **Visibility culling**: Para renderização quando não visível
- **Cache otimizado**: Propriedades de cena são cached para evitar recálculos
- **Rendering eficiente**: Atualização de textura apenas quando necessário

### 🔧 Configurações Avançadas
- **Target FPS**: Configure FPS alvo para economizar recursos
- **Frame Skip**: Habilite/desabilite pulo de frames
- **GPU Ready**: Preparado para compute shaders (implementação futura)

## 📋 Propriedades Disponíveis

### Propriedades do Rive (iguais ao RiveViewer)
- `file_path` (String): Caminho para arquivo .riv
- `fit` (int): Modo de ajuste (Fill, Contain, Cover, etc.)
- `alignment` (int): Alinhamento (TopLeft, Center, BottomRight, etc.)
- `disable_press` (bool): Desabilitar eventos de clique
- `disable_hover` (bool): Desabilitar eventos de hover
- `paused` (bool): Pausar animações

### Propriedades de Performance (novas)
- `target_fps` (float): FPS alvo (padrão: 60.0)
- `frame_skip_enabled` (bool): Habilitar pulo de frames (padrão: true)

### Propriedades Sprite2D (herdadas)
- `scale` (Vector2): Escala do sprite
- `rotation` (float): Rotação em radianos
- `position` (Vector2): Posição no espaço 2D
- `modulate` (Color): Cor de modulação
- `visible` (bool): Visibilidade
- E todas as outras propriedades do Sprite2D

## 🎮 Sinais (Signals)

### Sinais do Rive
- `pressed(position: Vector2)`: Clique detectado
- `released(position: Vector2)`: Clique liberado
- `scene_property_changed(scene, property, new_value, old_value)`: Propriedade de cena mudou

## 🔧 Métodos Principais

### Navegação
```gdscript
# Ir para artboard específico
rive_sprite.go_to_artboard(artboard_ref)

# Ir para cena específica
rive_sprite.go_to_scene(scene_ref)

# Ir para animação específica
rive_sprite.go_to_animation(animation_ref)
```

### Interação com Mouse
```gdscript
# Simular eventos de mouse
rive_sprite.press_mouse(Vector2(100, 100))
rive_sprite.release_mouse(Vector2(100, 100))
rive_sprite.move_mouse(Vector2(150, 150))
```

### Getters de Informação
```gdscript
# Obter objetos Rive
var file = rive_sprite.get_file()
var artboard = rive_sprite.get_artboard()
var scene = rive_sprite.get_scene()
var animation = rive_sprite.get_animation()

# Tempo decorrido
var elapsed = rive_sprite.get_elapsed_time()
```

## 📝 Exemplo de Uso em GDScript

```gdscript
extends Node2D

@onready var rive_sprite = $RiveSprite2D

func _ready():
    # Configurar arquivo Rive
    rive_sprite.file_path = "res://animations/character.riv"
    
    # Configurar performance
    rive_sprite.target_fps = 30.0  # Economizar recursos
    rive_sprite.frame_skip_enabled = true
    
    # Configurar fit e alignment
    rive_sprite.fit = 2  # Contain
    rive_sprite.alignment = 5  # Center
    
    # Usar transformações normais do Sprite2D
    rive_sprite.scale = Vector2(2.0, 2.0)  # 2x maior
    rive_sprite.rotation = deg_to_rad(45)  # 45 graus
    rive_sprite.position = Vector2(400, 300)  # Centro da tela
    
    # Conectar sinais
    rive_sprite.pressed.connect(_on_rive_pressed)
    rive_sprite.scene_property_changed.connect(_on_scene_property_changed)

func _on_rive_pressed(position: Vector2):
    print("Rive clicado na posição: ", position)
    
    # Exemplo: mudar escala quando clicado
    var tween = create_tween()
    tween.tween_property(rive_sprite, "scale", Vector2(2.5, 2.5), 0.2)
    tween.tween_property(rive_sprite, "scale", Vector2(2.0, 2.0), 0.2)

func _on_scene_property_changed(scene, property, new_value, old_value):
    print("Propriedade mudou: ", property, " de ", old_value, " para ", new_value)

# Exemplo de controle de animação
func play_idle_animation():
    var artboard = rive_sprite.get_artboard()
    if artboard:
        var idle_anim = artboard.get_animation("idle")
        if idle_anim:
            rive_sprite.go_to_animation(idle_anim)

func play_walk_animation():
    var artboard = rive_sprite.get_artboard()
    if artboard:
        var walk_anim = artboard.get_animation("walk")
        if walk_anim:
            rive_sprite.go_to_animation(walk_anim)
```

## 🎯 Casos de Uso Ideais

### 1. Personagens de Jogo
```gdscript
# RiveSprite2D é perfeito para personagens animados
extends CharacterBody2D

@onready var character_sprite = $RiveSprite2D

func _ready():
    character_sprite.file_path = "res://characters/hero.riv"
    character_sprite.scale = Vector2(0.5, 0.5)  # Ajustar tamanho

func _physics_process(delta):
    # Movimento normal
    velocity = Vector2(100, 0) if Input.is_action_pressed("move_right") else Vector2.ZERO
    move_and_slide()
    
    # Animação baseada no movimento
    if velocity.length() > 0:
        play_walk_animation()
    else:
        play_idle_animation()
```

### 2. UI Interativa
```gdscript
# Botões e elementos UI animados
extends Control

@onready var animated_button = $RiveSprite2D

func _ready():
    animated_button.file_path = "res://ui/button.riv"
    animated_button.pressed.connect(_on_button_pressed)
    
    # Posicionar como qualquer Sprite2D
    animated_button.position = Vector2(200, 100)

func _on_button_pressed(pos):
    # Lógica do botão
    print("Botão animado pressionado!")
```

### 3. Efeitos Visuais
```gdscript
# Partículas e efeitos especiais
extends Node2D

func create_explosion_effect(pos: Vector2):
    var explosion = preload("res://RiveSprite2D.tscn").instantiate()
    explosion.file_path = "res://effects/explosion.riv"
    explosion.position = pos
    explosion.scale = Vector2(3.0, 3.0)
    add_child(explosion)
    
    # Auto-remover após animação
    await get_tree().create_timer(2.0).timeout
    explosion.queue_free()
```

## 🔄 Migração do RiveViewer

Para migrar do RiveViewer para RiveSprite2D:

1. **Substitua o nó**: Troque RiveViewer por RiveSprite2D
2. **Mantenha propriedades**: Todas as propriedades do Rive continuam iguais
3. **Aproveite Sprite2D**: Use `scale`, `rotation`, `position` ao invés de propriedades de tamanho customizadas
4. **Configure performance**: Ajuste `target_fps` e `frame_skip_enabled` conforme necessário

## ⚡ Dicas de Performance

### Otimização Automática
- O RiveSprite2D automaticamente para de renderizar quando não visível
- Frame skipping é habilitado por padrão para economizar recursos
- Cache inteligente evita recálculos desnecessários

### Configuração Manual
```gdscript
# Para animações simples, reduza o FPS
rive_sprite.target_fps = 24.0

# Para animações críticas, use FPS máximo
rive_sprite.target_fps = 60.0
rive_sprite.frame_skip_enabled = false

# Para muitos sprites na tela
rive_sprite.target_fps = 15.0
rive_sprite.frame_skip_enabled = true
```

## 🚀 Futuras Otimizações

O RiveSprite2D está preparado para:
- **Compute Shaders**: Renderização GPU para performance extrema
- **Instancing**: Múltiplos sprites compartilhando recursos
- **LOD System**: Nível de detalhe baseado na distância
- **Async Loading**: Carregamento assíncrono de arquivos .riv

---

**Resultado**: Um nó Rive otimizado que combina a facilidade do Sprite2D com o poder das animações Rive! 🎉
