if #app.sprites < 1 then
  return app.alert("You should have at least one sprite opened")
end

local sprites = app.sprites

----------------------------------------------------------------
-- Determine maximum bounds
----------------------------------------------------------------
local bounds = Rectangle()

for _, sprite in ipairs(sprites) do
  bounds = bounds:union(sprite.bounds)
end

----------------------------------------------------------------
-- Create destination sprite
----------------------------------------------------------------
local dst = Sprite(bounds.width, bounds.height)
dst.filename = "merged_sprite"

----------------------------------------------------------------
-- Remove default layer/frame
----------------------------------------------------------------
app.transaction(function()
  if #dst.layers > 0 then
    dst:deleteLayer(dst.layers[1])
  end

  while #dst.frames > 1 do
    dst:deleteFrame(dst.frames[#dst.frames])
  end
end)

----------------------------------------------------------------
-- Utilities
----------------------------------------------------------------
local function copyUserData(srcObj, dstObj)
  if srcObj.properties and dstObj.properties then
    for k,v in pairs(srcObj.properties) do
      dstObj.properties[k] = v
    end
  end

  if srcObj.color then
    dstObj.color = srcObj.color
  end

  if srcObj.data then
    dstObj.data = srcObj.data
  end
end

----------------------------------------------------------------
-- Main merge
----------------------------------------------------------------
app.transaction(function()

  local globalFrameIndex = 1

  for _, src in ipairs(sprites) do

    ----------------------------------------------------------------
    -- Create corresponding layers
    ----------------------------------------------------------------
    local layerMap = {}

    local function cloneLayerTree(srcLayer, parent)
      local newLayer

      if srcLayer.isGroup then
        newLayer = dst:newGroup()
        newLayer.name = srcLayer.name
      else
        newLayer = dst:newLayer()
        newLayer.name = srcLayer.name
      end

      if parent then
        newLayer.parent = parent
      end

      newLayer.opacity = srcLayer.opacity
      newLayer.blendMode = srcLayer.blendMode
      newLayer.isVisible = srcLayer.isVisible

      layerMap[srcLayer] = newLayer

      copyUserData(srcLayer, newLayer)

      if srcLayer.isGroup then
        for _, child in ipairs(srcLayer.layers) do
          cloneLayerTree(child, newLayer)
        end
      end
    end

    for _, layer in ipairs(src.layers) do
      cloneLayerTree(layer, nil)
    end

    ----------------------------------------------------------------
    -- Copy frames/cels
    ----------------------------------------------------------------
    local spriteStartFrame = globalFrameIndex

    for frameIndex, srcFrame in ipairs(src.frames) do

      if globalFrameIndex > #dst.frames then
        dst:newEmptyFrame()
      end

      local dstFrame = dst.frames[globalFrameIndex]
      dstFrame.duration = srcFrame.duration

      copyUserData(srcFrame, dstFrame)

      local function copyLayerCels(srcLayer)
        if srcLayer.isGroup then
          for _, child in ipairs(srcLayer.layers) do
            copyLayerCels(child)
          end
          return
        end

        local srcCel = srcLayer:cel(frameIndex)

        if srcCel then
          local dstLayer = layerMap[srcLayer]

          local newCel = dst:newCel(
            dstLayer,
            dstFrame,
            srcCel.image,
            srcCel.position
          )

          newCel.opacity = srcCel.opacity

          copyUserData(srcCel, newCel)
        end
      end

      for _, layer in ipairs(src.layers) do
        copyLayerCels(layer)
      end

      globalFrameIndex = globalFrameIndex + 1
    end

    local spriteEndFrame = globalFrameIndex - 1

    ----------------------------------------------------------------
    -- Copy frame tags
    ----------------------------------------------------------------
    for _, tag in ipairs(src.tags) do
      local newTag = dst:newTag(
        spriteStartFrame + tag.fromFrame.frameNumber - 1,
        spriteStartFrame + tag.toFrame.frameNumber - 1
      )

      newTag.name = tag.name
      newTag.animationDirection = tag.animationDirection
      newTag.color = tag.color

      copyUserData(tag, newTag)
    end

    ----------------------------------------------------------------
    -- Copy slices
    ----------------------------------------------------------------
    for _, slice in ipairs(src.slices) do
      local newSlice = dst:newSlice(slice.name)

      copyUserData(slice, newSlice)

      for _, key in ipairs(slice.keys) do

        local newFrameNumber =
          spriteStartFrame + key.frameNumber - 1

        local newKey = newSlice:newKey(newFrameNumber)

        newKey.bounds = Rectangle(
          key.bounds.x,
          key.bounds.y,
          key.bounds.width,
          key.bounds.height
        )

        if key.center then
          newKey.center = Rectangle(
            key.center.x,
            key.center.y,
            key.center.width,
            key.center.height
          )
        end

        if key.pivot then
          newKey.pivot = Point(
            key.pivot.x,
            key.pivot.y
          )
        end

        copyUserData(key, newKey)
      end
    end
  end
end)

app.activeSprite = dst
app.activeFrame = dst.frames[1]

app.refresh()

app.alert("Merged sprites successfully!")
