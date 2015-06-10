package org.wso2.iot.azdroid;

import javax.ws.rs.*;

@Path("/azdroid")
public class AzdroidCommander {

	@GET
	@Path("move/{direction}")
	public void move(@PathParam("direction") String direction){
		
		Direction dir = Direction.valueOf(direction.toUpperCase());
		if (dir != null) {
			MqttConnectionManager.getInstance().publish("" + dir.getDirectionId());
		}
	}

}