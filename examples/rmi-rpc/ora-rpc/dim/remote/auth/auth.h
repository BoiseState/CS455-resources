#define AUTHNONE(rqstp,transp,uid,gid,gidlen,gidlist) { \
                fprintf(stderr, "not checking credentials\n"); \
            }  
#define AUTHDES(rqstp,transp,uid,,gid,gidlen,gidlist) { \
                switch (rqstp->rq_cred.oa_flavor) { \
                case AUTH_DES:  { \
                        struct authdes_cred *des_cred = \
                          (struct authdes_cred *) rqstp->rq_clntcred; \
                        if (! netname2user(des_cred->adc_fullname.name, \
                          &uid, &gid, &gidlen, gidlist)) { \
                                /* could use syslog(3) here */ \
                                fprintf(stderr, "unknown user: %s\n", \
                                  des_cred->adc_fullname.name); \
                                svcerr_systemerr(transp); \
                                return; \
                        } \
                        break; \
                    } \
                case AUTH_NULL: \
                default: \
                        /* could use syslog(3) here */ \
                        svcerr_weakauth(transp); \
                        return; \
                } \
            }
#define AUTHUNIX(rqstp,transp,uid,gid,gidlen,gidlist) { \
                switch (rqstp->rq_cred.oa_flavor) {   \
                case AUTH_UNIX: {   /* AUTH_SYS will obsolete this */ \
                        struct authunix_parms * unix_cred =   \
                                (struct authunix_parms *)rqstp->rq_clntcred; \
                        uid = unix_cred->aup_uid;   \
                        gid = unix_cred->aup_gid;   \
                        gidlen = unix_cred->aup_len;   \
                        gidlist = &(unix_cred->aup_gids);   \
                        break;   \
                        } \
                case AUTH_NULL:   \
                default:   \
                        /* could use syslog(3) here */ \
                        svcerr_weakauth(transp);   \
                        return;   \
                } \
            }
