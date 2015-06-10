package org.wso2.iot.azdroid;

public enum Direction {

	FORWARD(8), REVERSE(2), RIGHT(6), LEFT(4), STOP(0);

	private int directionId;
	
	private Direction(int directionId) {
		this.directionId = directionId;
	}

	public int getDirectionId(){
		return directionId;
	}
}
