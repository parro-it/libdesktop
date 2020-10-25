import test from 'tape-async'
import {App,Window,Label,Textfield,Container,YogaFlexDirection} from '../index'
const app = App.create()


const win = new Window({style: {padding: {left:26,right:26,top:12,bottom:12}}},[   
    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:0,top:0}, minWidth:100},label: "Given Name"},[]),
        new Textfield({style: {margin:{left:12,top:0}},text: "Andrea"},[]),
    ]),
    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:0,top: 12}, minWidth:100},label: "Family Name"},[]),
        new Textfield({style: {margin:{left: 12,top: 12}},text: "Parodi"},[]),
    ]),

    new Container({style: {flexDirection: YogaFlexDirection.Row}},[
        new Label({style: {margin:{left:0,top: 12}, minWidth:100},label: "City"},[]),
        new Textfield({style: {margin:{left: 12,top: 12}},text: "Genoa"},[]),
    ]),
]);

app.start();