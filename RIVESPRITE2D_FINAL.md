# RiveSprite2D - Nó Rive Otimizado para Sprite2D

## ✅ **PROBLEMA RESOLVIDO**: RiveSprite2D agora funciona corretamente!

O **RiveSprite2D** é uma versão otimizada do RiveViewer que herda de **Sprite2D**, oferecendo **todas as funcionalidades do Rive** com melhor integração ao sistema de transformação do Godot.

## 🎯 **Principais Vantagens sobre RiveViewer**

### ✨ **Herança Nativa de Sprite2D**
- **Transformação intuitiva**: Use `scale`, `rotation`, `position` normalmente
- **Compatibilidade total**: Funciona com todos os sistemas que esperam Sprite2D
- **Interface familiar**: Mesma API que desenvolvedores Godot já conhecem

### ⚡ **Renderização Otimizada**
- **Mesma lógica do RiveViewer**: Mantém a estabilidade e confiabilidade
- **Inicialização correta**: Textura é criada automaticamente quando necessário
- **Cache eficiente**: Propriedades de cena são cached para evitar recálculos
- **Visibility culling**: Para renderização quando não visível

### 🔧 **Funcionalidades Completas**
- **Arquivos .riv**: Carregamento e reprodução completos
- **Timelines**: Controle total de animações
- **State machines**: Máquinas de estado interativas
- **Eventos de mouse**: Cliques e hover funcionam perfeitamente
- **Propriedades dinâmicas**: Inputs de cena são expostos no editor

## 📋 **Propriedades Disponíveis**

### **Propriedades do Rive** (idênticas ao RiveViewer)
- `file_path` (String): Caminho para arquivo .riv
- `fit` (int): Modo de ajuste (Fill, Contain, Cover, FitWidth, FitHeight, None, ScaleDown)
- `alignment` (int): Alinhamento (TopLeft, TopCenter, TopRight, CenterLeft, Center, CenterRight, BottomLeft, BottomCenter, BottomRight)
- `disable_press` (bool): Desabilitar eventos de clique
- `disable_hover` (bool): Desabilitar eventos de hover
- `paused` (bool): Pausar animações

### **Propriedades Sprite2D** (herdadas)
- `scale` (Vector2): Escala do sprite - **FUNCIONA NORMALMENTE!**
- `rotation` (float): Rotação em radianos
- `position` (Vector2): Posição no espaço 2D
- `modulate` (Color): Cor de modulação
- `visible` (bool): Visibilidade
- E todas as outras propriedades do Sprite2D

## 🎮 **Sinais (Signals)**

### **Sinais do Rive** (iguais ao RiveViewer)
- `pressed(position: Vector2)`: Clique detectado
- `released(position: Vector2)`: Clique liberado
- `scene_property_changed(scene, property, new_value, old_value)`: Propriedade de cena mudou

## 🔧 **API Completa**

### **Métodos de Navegação**
```gdscript
# Ir para artboard específico
rive_sprite.go_to_artboard(artboard_ref)

# Ir para cena específica
rive_sprite.go_to_scene(scene_ref)

# Ir para animação específica
rive_sprite.go_to_animation(animation_ref)
```

### **Interação com Mouse**
```gdscript
# Simular eventos de mouse
rive_sprite.press_mouse(Vector2(100, 100))
rive_sprite.release_mouse(Vector2(100, 100))
rive_sprite.move_mouse(Vector2(150, 150))
```

### **Getters de Informação**
```gdscript
# Obter objetos Rive
var file = rive_sprite.get_file()
var artboard = rive_sprite.get_artboard()
var scene = rive_sprite.get_scene()
var animation = rive_sprite.get_animation()

# Tempo decorrido
var elapsed = rive_sprite.get_elapsed_time()
```

## 📝 **Exemplo Prático de Uso**

```gdscript
extends Node2D

@onready var rive_sprite = $RiveSprite2D

func _ready():
    # Configurar arquivo Rive (igual ao RiveViewer)
    rive_sprite.file_path = "res://animations/character.riv"
    
    # Configurar fit e alignment (igual ao RiveViewer)
    rive_sprite.fit = 2  # Contain
    rive_sprite.alignment = 5  # Center
    
    # NOVA FUNCIONALIDADE: Usar transformações normais do Sprite2D!
    rive_sprite.scale = Vector2(2.0, 2.0)  # 2x maior - FUNCIONA!
    rive_sprite.rotation = deg_to_rad(45)  # 45 graus - FUNCIONA!
    rive_sprite.position = Vector2(400, 300)  # Centro da tela
    
    # Conectar sinais (igual ao RiveViewer)
    rive_sprite.pressed.connect(_on_rive_pressed)
    rive_sprite.scene_property_changed.connect(_on_scene_property_changed)

func _on_rive_pressed(position: Vector2):
    print("Rive clicado na posição: ", position)
    
    # Exemplo: animar escala quando clicado
    var tween = create_tween()
    tween.tween_property(rive_sprite, "scale", Vector2(2.5, 2.5), 0.2)
    tween.tween_property(rive_sprite, "scale", Vector2(2.0, 2.0), 0.2)

func _on_scene_property_changed(scene, property, new_value, old_value):
    print("Propriedade mudou: ", property, " de ", old_value, " para ", new_value)
```

## 🔄 **Migração do RiveViewer para RiveSprite2D**

### **Passo a Passo**
1. **Substitua o nó**: Troque RiveViewer por RiveSprite2D na cena
2. **Mantenha todas as propriedades**: `file_path`, `fit`, `alignment`, etc. continuam iguais
3. **Remova propriedades de tamanho**: Não precisa mais definir tamanho manualmente
4. **Use transformações Sprite2D**: Aproveite `scale`, `rotation`, `position`

### **Vantagens da Migração**
- ✅ **Funcionalidade idêntica**: Tudo que funcionava no RiveViewer funciona aqui
- ✅ **Transformações nativas**: `scale`, `rotation` funcionam como esperado
- ✅ **Melhor performance**: Otimizações internas para Sprite2D
- ✅ **Compatibilidade**: Funciona com sistemas que esperam Sprite2D

## 🎯 **Casos de Uso Perfeitos**

### **1. Personagens de Jogo**
```gdscript
extends CharacterBody2D

@onready var character_sprite = $RiveSprite2D

func _ready():
    character_sprite.file_path = "res://characters/hero.riv"
    character_sprite.scale = Vector2(0.8, 0.8)  # Ajustar tamanho facilmente

func _physics_process(delta):
    # Movimento normal
    if Input.is_action_pressed("move_right"):
        velocity.x = 100
        character_sprite.scale.x = abs(character_sprite.scale.x)  # Virar sprite
    elif Input.is_action_pressed("move_left"):
        velocity.x = -100
        character_sprite.scale.x = -abs(character_sprite.scale.x)  # Virar sprite
    
    move_and_slide()
```

### **2. UI Interativa**
```gdscript
extends Control

@onready var animated_button = $RiveSprite2D

func _ready():
    animated_button.file_path = "res://ui/button.riv"
    animated_button.pressed.connect(_on_button_pressed)
    
    # Posicionar e escalar como qualquer Sprite2D
    animated_button.position = Vector2(200, 100)
    animated_button.scale = Vector2(1.5, 1.5)

func _on_button_pressed(pos):
    print("Botão animado pressionado!")
    
    # Animar o botão
    var tween = create_tween()
    tween.tween_property(animated_button, "scale", Vector2(1.3, 1.3), 0.1)
    tween.tween_property(animated_button, "scale", Vector2(1.5, 1.5), 0.1)
```

### **3. Efeitos Visuais**
```gdscript
extends Node2D

func create_explosion(pos: Vector2, size: float = 1.0):
    var explosion = preload("res://RiveSprite2D.tscn").instantiate()
    explosion.file_path = "res://effects/explosion.riv"
    explosion.position = pos
    explosion.scale = Vector2(size, size)  # Tamanho variável!
    add_child(explosion)
    
    # Auto-remover após animação
    await get_tree().create_timer(2.0).timeout
    explosion.queue_free()
```

## ❓ **Sobre o target_fps removido**

**Por que foi removido?**
- **Desnecessário**: O Godot já gerencia FPS automaticamente
- **Confuso**: Não oferecia benefício real de performance
- **Simplicidade**: RiveSprite2D agora funciona exatamente como RiveViewer
- **Estabilidade**: Menos código = menos bugs

**O RiveSprite2D agora:**
- ✅ Renderiza na mesma frequência que o RiveViewer
- ✅ Usa a mesma lógica de frame skipping automático
- ✅ Performance otimizada sem configurações desnecessárias

## 🚀 **Resultado Final**

O **RiveSprite2D** é agora um **substituto perfeito** do RiveViewer com as seguintes vantagens:

### ✅ **Funcionalidades Completas**
- Carrega arquivos .riv perfeitamente
- Reproduz animações e timelines
- Suporta state machines interativas
- Eventos de mouse funcionam corretamente
- Propriedades dinâmicas expostas no editor

### ✅ **Integração Sprite2D**
- Transformações nativas (`scale`, `rotation`, `position`)
- Compatibilidade com sistemas existentes
- Interface familiar para desenvolvedores

### ✅ **Performance Otimizada**
- Mesma estabilidade do RiveViewer
- Renderização eficiente
- Sem configurações desnecessárias

---

## 🎉 **Conclusão**

O **RiveSprite2D** oferece a **melhor experiência** para usar animações Rive no Godot:
- **Funciona perfeitamente** como o RiveViewer
- **Transformações nativas** do Sprite2D
- **Código mais limpo** e estável
- **Migração simples** do RiveViewer existente

**Use RiveSprite2D quando quiser:**
- Transformações nativas do Sprite2D
- Compatibilidade com sistemas que esperam Sprite2D
- Interface mais familiar e intuitiva

**Use RiveViewer quando quiser:**
- Controle total sobre tamanho e posicionamento
- Funcionalidades específicas do Control
- Sistemas UI complexos
