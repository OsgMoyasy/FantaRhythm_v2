#pragma once

//FlipSet/////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> FlipSet<T>::~FlipSet() {
	for (auto& effect : usingEffect) {
		delete effect.second;
	}
}
template<typename T> void FlipSet<T>::set(T name, const FilePath& path, int xFlipWidth, int yFlipHeight, int xDraw, int yDraw) {
	usingEffect[name] = new FlipEffect(path, xFlipWidth, yFlipHeight, xDraw, yDraw);
}
template<typename T> FlipEffect* FlipSet<T>::operator[](T name) {
	return usingEffect[name];
}
template<typename T> void FlipSet<T>::draw() {
	for (auto& effect : usingEffect) {
		effect.second->draw();
	}
}
