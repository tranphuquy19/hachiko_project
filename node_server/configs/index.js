module.exports = {
    server: {
        domain: process.env.SERVER_DOMAIN,
        mcuRoute: process.env.MCU_ROUTE
    },

    mongoDb: {
        hostname: process.env.MONGODB_HOSTNAME,
        databaseName: process.env.MONGODB_DATABASENAME,
        username: process.env.MONGODB_USERNAME,
        password: process.env.MONGODB_PASSWORD
    },

    homeConfig: {
        timeZone: process.env.TIME_ZONE,
        timeFormat: process.env.TIME_FORMAT,
        longitude: process.env.LONGITUDE,
        latitude: process.env.LATITUDE
    }
};