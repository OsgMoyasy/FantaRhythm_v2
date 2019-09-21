#pragma once

//FlipMap/////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> FlipMap<T>::~FlipMap() {
	for (auto& effect : usingEffect) {
		delete effect.second;
	}
}
template<typename T> void FlipMap<T>::set(T name, const FilePath& path, int xFlipWidth, int yFlipHeight, int xDraw, int yDraw) {
	usingEffect[name] = new FlipEffect(path, xFlipWidth, yFlipHeight, xDraw, yDraw);
}
template<typename T> FlipEffect* FlipMap<T>::get(T name) {
	return usingEffect[name];
}
template<typename T> void FlipMap<T>::draw() {
	for (auto& effect : usingEffect) {
		effect.second->draw();
	}
}
