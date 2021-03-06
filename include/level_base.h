#ifndef LEVEL_BASE_H
#define LEVEL_BASE_H

#include <memory>

#include "SFML/Graphics.hpp"

#include "level.h"
#include "nine_patch.h"

/**
 * @brief A base class that handles most draw-&-build logic, joint-breakage and rendering
 * 
 */
class LevelBase : public LevelStage {
public:
  struct LevelBaseDef {
    std::string levelName;
    std::string levelConfig;
    std::string textureBundle;
    float plankDensity;
    float plankWidth;
    float plankMaxLength;
    float plankDamping;
    std::string plankTexture;
    std::string nodeTexture;
    float jointBreakageForceSq;
    long initialAnger;
    long angerThresholds[2];
  };
  struct Node {
    sf::Vector2f p;
    std::size_t count = 0;
    b2Body *body = nullptr;
    float hw = 0;
  };
  using NodeList = std::list<Node>;
  struct Plank {
    NinePatchSprite sprite;
    NodeList::iterator start;
    NodeList::iterator end;
  };
  enum State {
    ENDED,
    STARTED,
    RESTARTING,
  };
  struct PlayerState {
    sf::Clock lastHit;
    long anger;
    enum Mood {
      HAPPY = 0,
      ASLEEP = 1,
      AWAKE = 2,
      SAD = 3,
    };
    Mood mood;
    const static char *MOOD_NAMES[4];
  };

protected:
  LevelBaseDef def;
  State state;
  PlayerState player;
  BundledTexture bundle;
  sf::Font font;
  NinePatchSprite sprite;
  sf::Text messages;
  sf::Vector2f start;
  bool clicked;
  NodeList::iterator startNode;
  NodeList nodes;
  std::vector<Plank> sprites;
  sf::Sprite node;
  void draw(sf::RenderTarget &target, const sf::RenderStates &states) const override;
  void bindJoint(Plank &p, b2Body *body, float hw);
  std::pair<b2Body *, float> addBody(Plank &p);
  NodeList::iterator findNode(sf::Vector2f pos, float scale);
  std::list<b2Joint *> joints;

  void breakJoints();
  void stepPlayerMood();
  void stepPlayerState();

public:
  LevelBase(StageManager &manager, AssetManager &assets,
            const std::string &config, const TiledWorldDef::RenDef &rendering);
  void prepare(bool paused) override;
  void step(float delta) override;
  bool rainStep(float delta) override;
  LevelBaseDef loadConfig();
  bool onEvent(sf::Event &event) override;
  bool onMousedown(B2ObjectInfo &name) override;
  bool onHover(sf::Vector2f position) override;
  void setMessage(const std::string &message);
  void restart();
  virtual void onPlayerMood(PlayerState::Mood mood);
};

#endif /* !LEVEL_BASE_H */