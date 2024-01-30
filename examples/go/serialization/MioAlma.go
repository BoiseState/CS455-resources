//Author: Amit Jain & Paul Ellis

package main

import (
	"time"
	"fmt"
)

//Unlike in Java there is no keyword to privatize variables
//From serialization. The only way is to make them "private"
//Which means that you leave the first letter of the name
//lower case. Upper case variable names are "public"
type MioAlma struct{
	Name string
	UniqueID int64
	Birthdate time.Time
	interests string
}

func NewMioAlma(name string, uid int64) *MioAlma{
	m := MioAlma{Name:name, UniqueID:uid, Birthdate:time.Now(), interests:"daydreaming"}
	return &m
}

func (m *MioAlma)GetName() string{
	return m.Name
}

func (m *MioAlma)SetName(newName string){
	m.Name = newName
}

func (m *MioAlma)GetUniqueId() int64{
	return m.UniqueID
}

func (m *MioAlma)GetBirthdate() time.Time{
	return m.Birthdate
}

func (m *MioAlma)ToString() string{
	msg := fmt.Sprintf("MioAlma [name=%s, uniqueId=%d, birthdate=%s, interests=%s]",
			m.Name,
			m.UniqueID,
			m.Birthdate.Format(time.UnixDate),
			m.interests)
	return msg
}